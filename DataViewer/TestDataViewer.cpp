#include<stdio.h>
#include<Windows.h>
#include<string>

class FileReader {
private:
	std::wstring target_file_path;
	HANDLE hFile;
	HANDLE hMap;
	LPVOID base_address;
	LONGLONG file_size;

public:
	FileReader(std::wstring wFile) {
		target_file_path = wFile;
		hFile = INVALID_HANDLE_VALUE;
	}

	~FileReader() {
		if (base_address) {
			UnmapViewOfFile(base_address);
		}

		if (hMap) {
			CloseHandle(hMap);
		}

		if (hFile != INVALID_HANDLE_VALUE) {
			CloseHandle(hFile);
		}
	}

	bool Open() {
		if (!target_file_path.length()) {
			return false;
		}

		hFile = CreateFileW(target_file_path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			return false;
		}

		if (!GetFileSizeEx(hFile, (LARGE_INTEGER *)&file_size)) {
			return false;
		}

		hMap = CreateFileMappingW(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (!hMap) {
			return false;
		}

		base_address = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
		if (!base_address) {
			return false;
		}

		return true;
	}

	bool Read(LONGLONG uOffset, LONGLONG uSize) {
		if (!base_address) {
			return false;
		}

		printf("%lld\n", file_size);

		if (uOffset + uSize > file_size) {
			return false;
		}

		BYTE *mem = (BYTE *)base_address;

		for (LONGLONG i = 0; i < uSize; i++) {
			printf("%02X ", mem[uOffset + i]);
		}
		puts("");


		return true;
	}
};

#define TEST_WZ_PATH L"C:\\Users\\elfenlied\\Desktop\\JMS\\Data\\String\\String_000.wz"

int main() {
	FileReader fr(TEST_WZ_PATH);

	if(!fr.Open()){
		puts("File Not Opened");
		return 1;
	}

	fr.Read(0, 100);

	return 0;
}