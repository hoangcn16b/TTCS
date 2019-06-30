#include <iostream> 
#include <fstream> 
#include <string> 
#include <iomanip> 
#include <stdio.h> 
#include <conio.h> 
#include <sstream> 
#include <Windows.h> 
#include <cstdlib>
#include <io.h>
#include <codecvt>
#include <fcntl.h>
using namespace std;

struct date {
	int day, month, year;
};

struct NhanVien {
	wstring hoTen;
	wstring chucVu;
	struct date date;
	float heSoLuong;
};


struct node {
	NhanVien data;
	struct node *next;
};

typedef struct node NODE;

struct list {
	NODE *pHead;
	NODE *pTail;
};

typedef struct list LIST;

void initList(LIST &l) {
	l.pHead = NULL;
	l.pTail = NULL;
}

NODE *initNode(NhanVien x) {
	NODE *p = new NODE;
	if (p == NULL)
	{
		cout << "\nCấp phát thất bại !";
		return NULL;
	}
	p->data = x;
	p->next = NULL;
	return p;
}

void addHeadList(LIST &l, NODE *p)
{
	if (l.pHead == NULL)
		l.pHead = l.pTail = p;
	else
	{
		p->next = l.pHead;
		l.pHead = p;
	}
}

void addEndList(LIST &l, NODE *p)
{
	if (l.pHead == NULL)
		l.pHead = l.pTail = p;
	else {
		l.pTail->next = p;
		l.pTail = p;
	}
}

// Đọc dữ liệu ngày tháng năm từ text 
void readDate(wifstream &filein, date &date)
{
	filein >> date.day;
	filein.seekg(0, 1);
	filein >> date.month;
	filein.seekg(0, 1);
	filein >> date.year;
}

void readFileFromText(wifstream &filein, NhanVien &nv)
{
	//HO TEN 
	getline(filein, nv.hoTen, L'|');
	filein.seekg(1, 1);
	//CHUC VU 
	getline(filein, nv.chucVu, L'|');
	filein.seekg(1, 1);
	//DATE 
	readDate(filein, nv.date);
	filein.seekg(1, 1);
	//HE SO LUONG 
	filein >> nv.heSoLuong;

	wstring temp;
	getline(filein, temp);
}

void readList(wifstream &filein, LIST &l)
{
	while (!filein.eof()) {
		NhanVien nv;
		readFileFromText(filein, nv);
		NODE *p = initNode(nv);
		addEndList(l, p);
	}
}


void ouputNhanVien(NhanVien &nv) {


	wcout << nv.hoTen
		<< setw(18) << nv.chucVu
		<< setw(10) << nv.date.day << "/" << nv.date.month << "/" << nv.date.year
		<< setw(18) << nv.heSoLuong;

}
void outputListNhanVien(LIST &l) {
	wcout << L"\n\t\t\tDanh sách nhân viên\n";
	wcout << L"\n--------------------------------------------------------------------------";
	wcout << L"\n Họ và tên             |     Chức vụ     |     Date     |     Hệ số lương\n";
	wcout << L"\n--------------------------------------------------------------------------";
	int dem = 1;
	for (NODE *k = l.pHead; k != NULL; k = k->next)
	{
		wcout << endl << endl;
		ouputNhanVien(k->data);

	}
}
//=====================================================
// function compare
//=====================================================
int compare_2_string(wchar_t *array1, wchar_t *array2)
{
	int i;
	int response = 0;
	i = 0;

	while (array1[i] == array2[i] && response == 0)
	{
		if (array1[i] == '\0' || array2[i] == '\0') {
			response = 1;
		}
		i++;
	}

	return response;
}


//=====================================================
//- SORT LIST
//=====================================================
void change2Node(NhanVien &x, NhanVien &y)
{
	NhanVien temp = x;
	x = y;
	y = temp;
}
//THEO NGÀY THÁNG NĂM
bool compare2Date(date &date1, date &date2)
{
	if (date1.year < date2.year)
		return true;
	else if (date1.year == date2.year && date1.month < date2.month)
		return true;
	else if (date1.year == date2.year && date1.month == date2.month && date1.day < date2.day)
		return true;
	return false;
}

void sortByDate(LIST &l)
{
	for (NODE *k = l.pHead; k != NULL; k = k->next)
	{
		for (NODE *h = k->next; h != NULL; h = h->next)
		{
			if (compare2Date(k->data.date, h->data.date))
				change2Node(k->data, h->data);
		}
	}
}
//CHỨC VỤ

bool compare2Position(wstring &str1, wstring &str2) {
	int vitri1 = -1;
	int vitri2 = -1;

	if (str1 == L"Nhân viên") {
		vitri1 = 0;
	}
	else if (str1 == L"Thư ký") {
		vitri1 = 1;
	}
	else if (str1 == L"Giám đốc") {
		vitri1 = 2;
	}

	if (str2 == L"Nhân viên") {
		vitri2 = 0;
	}
	else if (str2 == L"Thư ký") {
		vitri2 = 1;
	}
	else if (str2 == L"Giám đốc") {
		vitri2 = 2;
	}


	if (vitri1 < vitri2) {
		return true;
	}
	else {
		return false;
	}


	return false;
}

void sortByPosition(LIST &l)
{
	for (NODE *k = l.pHead; k != NULL; k = k->next)
	{
		for (NODE *h = k->next; h != NULL; h = h->next)
		{
			if (compare2Position(k->data.chucVu, h->data.chucVu))
				change2Node(k->data, h->data);
		}
	}
}

//HỆ SỐ LƯƠNG
void sortBySalary(LIST &l) {
	for (NODE *k = l.pHead; k != NULL; k = k->next) {
		for (NODE *h = k->next; h != NULL; h = h->next) {
			if (k->data.heSoLuong > h->data.heSoLuong)
				change2Node(k->data, h->data);
		}
	}
}//=====================================================
 //- THÊM NHÂN VIÊN - VÀ SẮP XẾP
 //=====================================================
void input(NhanVien &nv)
{
	wcin.ignore();
	wcout << L"\nHọ tên nhân viên: ";
	getline(wcin, nv.hoTen);
	wcout << L"\n========";
	//option 
	wcout << L"\nChọn chức vụ cho nhân viên: ";
	//wcin >> nv.chucVu;
	wchar_t selection;

	wcout << L"\n========";
	wcout << L"\n 1 - Nhân viên";
	wcout << L"\n 2 - Thư ký";
	wcout << L"\n 3 - Giám đốc";
	wcout << L"\n\n Nhập số theo chức vụ :";
	wcin >> selection;

	//======
	switch (selection)

	{
	case '1':
		nv.chucVu = L"Nhân viên";
		break;
	case '2':
		nv.chucVu = L"Thư ký";
		break;
	case '3':
		nv.chucVu = L"Giám đốc";
		break;
	default:
		break;
	}

	wcout << L"\nChức vụ bạn chọn là :" << nv.chucVu;
	wcout << L"\n========";
	wcout << L"\nNhập ngày tháng năm sinh: ";
	do {
		fflush(stdin);
		wcout << L"\nNgay sinh: ";
		wcin >> nv.date.day;
	} while (nv.date.day > 31 || nv.date.day < 1);
	do {
		fflush(stdin);
		wcout << L"\nThang sinh: ";
		wcin >> nv.date.month;
	} while (nv.date.month > 12 || nv.date.month < 1);
	do {
		fflush(stdin);
		wcout << L"\nNam sinh: ";
		wcin >> nv.date.year;
	} while (nv.date.year > 2016 || nv.date.year < 1900);
	wcout << L"\n========";
	wcout << L"\nNhập hệ số lương: ";
	wcin >> nv.heSoLuong;
}

void createNhanVien(LIST &l)
{
	NhanVien nv;
	wcout << L"Nhập thông tin nhân viên cần thêm :";
	input(nv);
	NODE *t = initNode(nv);
	addEndList(l, t);
}

//=====================================================
//- XOÁ
//=====================================================


//=====================================================
//- CẮT TÊN -- CHECK INT -- CHECK FLOAT -- CHECK STRING
//=====================================================
wchar_t* lowercaseChar(wchar_t* str) {
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] = str[i] + 32;
		}
		++i;
	}
	return str;
}

int convertIntFromString(wstring str)
{
	int num = 0, size = str.length();
	for (int i = 0, j = size; i < size; i++) {
		num += (str[--j] - '0')*pow(10, i);
	}
	return num;
}
float convertFloatFromString(const wchar_t* s) {
	float rez = 0, fact = 1;
	if (*s == '-') {
		s++;
		fact = -1;
	};
	for (int point_seen = 0; *s; s++) {
		if (*s == '.') {
			point_seen = 1;
			continue;
		};
		int d = *s - '0';
		if (d >= 0 && d <= 9) {
			if (point_seen) fact /= 10.0f;
			rez = rez * 10.0f + (float)d;
		};
	};
	return rez * fact;
};
//BOOL
bool isFloat(wstring s)
{
	wistringstream iss(s);
	float dummy;
	iss >> noskipws >> dummy;
	return iss && iss.eof();
}

bool isInteger(wstring &s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
		return false;
	wchar_t * p;
	wcstol(s.c_str(), &p, 10);
	return (*p == 0);
}



wstring splitName(wstring s) {
	int pos = 0;
	for (int i = s.length(); i >= 0; i--)
		if (s[i] == ' ') {
			pos = i;
			break;
		}
	return s.substr(pos + 1);
}
//=====================================================
//- TÌM KIẾM
//=====================================================

bool compare2StringName(wchar_t str1[], wchar_t str2[]) {
	const wchar_t* change1[] =
	{
		L"ÀÁẢÃẠĂẰẮẲẴẶÂẦẤẨẪẬ",
		L"ÒÓỎÕỌÔỒỐỔỖỘƠỜỚỞỠỢ",
		L"ÈÉẺẼẸÊỀẾỂỄỆ",
		L"ÙÚỦŨỤƯỪỨỬỮỰ",
		L"ÌÍỈĨỊ",
		L"ỲÝỶỸỴ",
		L"Đ"
	};

	const wchar_t* change2[] =
	{
		L"àáảãạăằắẳẵặâầấẩẫậ",
		L"òóỏõọôồốổỗộơờớởỡợ",
		L"èéẻẽẹêềếểễệ",
		L"ùúủũụưừứửữự",
		L"ìíỉĩị",
		L"ỳýỷỹỵ",
		L"đ"
	};

	for (int i = 0; i < 7; i++) {
		const wchar_t *str = change1[i];
		const wchar_t *needchange = change2[i];
		for (int j = 0; j < wcslen(str); j++) {
			for (int k = 0; k < wcslen(str1); k++) {
				if (str[j] == str1[k]) {
					str1[k] = needchange[j];
				}
			}
			for (int k = 0; k < wcslen(str2); k++) {
				if (str[j] == str2[k]) {
					str2[k] = needchange[j];
				}
			}
		}
	}

	int result = compare_2_string(lowercaseChar(str1), lowercaseChar(str2));

	if (result == 1) {
		return true;
	}
	else {
		return false;
	}
}





void findNhanVienByTenOrChucVu(LIST &l, wstring input) {
	for (NODE *k = l.pHead; k != NULL; k = k->next) {
		wstring ten = splitName(k->data.hoTen);
		wstring chucvu = k->data.chucVu;
		if (compare2StringName((wchar_t*)ten.c_str(), (wchar_t*)input.c_str())) {
			ouputNhanVien(k->data);
			wcout << endl << endl;
		}
		else if (compare2StringName((wchar_t*)chucvu.c_str(), (wchar_t*)input.c_str()))
		{
			ouputNhanVien(k->data);
			wcout << endl << endl;
		}
	}
}

void findNhanVienByYear(LIST &l, wstring input) {
	for (NODE *k = l.pHead; k != NULL; k = k->next) {
		int year = k->data.date.year;
		if (year == convertIntFromString(input)) {
			ouputNhanVien(k->data);
			wcout << endl << endl;
		}
	}
}

void findNhanVienByHsl(LIST &l, wstring input) {
	for (NODE *k = l.pHead; k != NULL; k = k->next) {
		float hsl = k->data.heSoLuong;
		if (hsl == convertFloatFromString((wchar_t*)input.c_str())) {
			ouputNhanVien(k->data);
			wcout << endl << endl;
		}
	}
}


void findNhanVien(LIST &l) {
	wstring input;
	wcin.ignore();
	wcout << L"Nhập dữ liệu bạn mún tìm :";
	getline(wcin, input);

	if (isInteger(input)) {
		if (convertIntFromString(input) > 1990 && convertIntFromString(input) < 2016) {
			wcout << L"\nBạn nhập năm !" << endl;
			findNhanVienByYear(l, input);
		}
		else {
			wcout << L"\nSố bạn nhập không hợp lệ !";
		}
	}
	else if (isFloat(input)) {
		wcout << L"\nBạn nhập float !" << endl;
		findNhanVienByHsl(l, input);
	}
	else
	{
		wcout << L"\nBạn nhập string !" << endl;
		findNhanVienByTenOrChucVu(l, input);
	}
}

void deleteByKeyWordName(LIST &l, wstring input) {
	NODE** p = &l.pHead;
	while (*p)
		if (compare2StringName((wchar_t*)(splitName((*p)->data.hoTen)).c_str(), (wchar_t*)input.c_str())) {
			NODE* tmp = (*p)->next;
			delete *p;
			*p = tmp;
		}
		else
			p = &(*p)->next;
}
void deleteByKeyWordDate(LIST &l, wstring year) {
	NODE** p = &l.pHead;
	while (*p)
		if ((*p)->data.date.year == convertIntFromString(year)) {
			NODE* tmp = (*p)->next;
			delete *p;
			*p = tmp;
		}
		else
			p = &(*p)->next;
}

void deleteByKeyWordHsl(LIST &l, wstring input) {
	NODE** p = &l.pHead;
	while (*p)
		if ((*p)->data.heSoLuong == convertFloatFromString((wchar_t*)input.c_str())) {
			NODE* tmp = (*p)->next;
			delete *p;
			*p = tmp;
		}
		else
			p = &(*p)->next;
}

void deleteNhanVien(LIST &l) {
	wstring input;
	wcin.ignore();
	wcout << L"Nhập dữ liệu bạn mún tìm :";
	getline(wcin, input);

	if (isInteger(input)) {
		if (convertIntFromString(input) > 1990 && convertIntFromString(input) < 2016) {
			wcout << L"\nBạn nhập năm !" << endl;
			deleteByKeyWordDate(l, input);
		}
		else {
			wcout << L"\nSố bạn nhập không hợp lệ !";
		}
	}
	else if (isFloat(input)) {
		wcout << L"\nBạn nhập float !" << endl;
		deleteByKeyWordHsl(l, input);

	}
	else
	{
		wcout << L"\nBạn nhập string !" << endl;
		deleteByKeyWordName(l, input);
	}
}

void writeFile(wofstream &fileout, NhanVien &nv)
{
	fileout << nv.hoTen << "| ";
	fileout << nv.chucVu << "| ";
	fileout << nv.date.day << "/" << nv.date.month << "/" << nv.date.year << "| ";
	fileout << nv.heSoLuong;
}
void login() {
	string pass = "";
	char ch;
	wcout << L"\n\n\n\n\t\t\t\t\tHệ thông quản lý danh sách nhân viên";
	wcout << L"\n\n\n\n\n\t\t\t\t\t\tNhập mật khẩu của bạn:";
	ch = _getch();
	while (ch != 13) {
		pass.push_back(ch);
		wcout << L'*';
		ch = _getch();
	}
	if (pass == "pass") {
		wcout << L"\n\n\n\t\t\t\t\t\t   Đang tải dữ liệu \n\t\t\t\t\t";
		for (int a = 1; a < 12; a++)
		{
			Sleep(500);
			wcout << L"...";
		}
		wcout << L"\n\n\n\t\t\t\t\t\tĐăng nhập thành công!! \n\n\n";

		system("PAUSE");
		system("CLS");
	}
	else {
		wcout << L"\nĐăng nhập thất bại...\n";
		login();
	}
}

void returnBack()
{
	wcout << L"Press enter back to menu";
	while (true) {
		fflush(stdin);
		char cPressKey;
		cin >> cPressKey;
		if (cPressKey == 10)
		{
			break;
		}
		else { continue; }
	}
}

void option() {
	//Đăng nhập trước khi vào hệ thống
	//login();
	NhanVien nv;
	LIST l;
	initList(l);
	while (true) {
	menuMain:
		system("cls");
		wcout << L"\n\n\t\t =========== CHÈN THEO TIÊU CHÍ ===========";
		wcout << L"\n1. Đọc file text danh sách nhân viên";
		wcout << L"\n2. Sắp xếp các nhân viên";
		wcout << L"\n3. Chèn nhân viên";
		wcout << L"\n4. Xoá nhân viên";
		wcout << L"\n5. Tìm kiếm nhân viên";
		wcout << L"\n6. Xuất danh sách nhân viên ra text";
		wcout << L"\n7. Xem danh sách nhân viên";
		wcout << L"\n0. Thoát theo phím bất kì!";
		wcout << L"\n\n\t\t =========== END ===========";
		while (true) {
			int choose;
			wcout << L"\nNhập lựa chọn: ";
			wcin >> choose;
			switch (choose) {
			case 1: {

				wifstream filein("NHANVIEN.txt");
				locale utf8_locale(locale(), new codecvt_utf8<wchar_t>);
				filein.imbue(utf8_locale);
				wchar_t bom = L'\0';
				filein.get(bom);


				readList(filein, l);
				wcout << L"Đọc file thành công !";
				filein.close();
				break;
			}
			case 2:
			{ system("cls");
			while (true)
			{
				wcout << L"\n\n\t\t =========== SẮP XẾP THEO TIÊU CHÍ ===========";
				wcout << L"\n1. Theo ngày tháng năm sinh";
				wcout << L"\n2. Theo chức vụ";
				wcout << L"\n3. Theo hệ số lương";
				wcout << L"\n0. Quay về màn hình chính!";
				wcout << L"\n\n\t\t =========== END ===========";
				while (true) {
					int choose;
					wcout << L"\nNhập lựa chọn: ";
					wcin >> choose;
					switch (choose)
					{
					case 0:
					{ goto menuMain; }
					case 1:
					{
						system("cls");
						sortByDate(l);
						outputListNhanVien(l);
						wcout << L"\nNhấn bất kì để quay về màn hình chính !";
						system("pause"); goto menuMain; break; }
					case 2: {
						system("cls");
						sortByPosition(l);
						outputListNhanVien(l);
						wcout << L"\nNhấn bất kì để quay về màn hình chính !";
						system("pause");
						goto menuMain;
						break;
					}
					case 3:
					{
						system("cls");
						sortBySalary(l);
						outputListNhanVien(l);
						wcout << L"\nNhấn bất kì để quay về màn hình chính !";
						system("pause");
						goto menuMain;
						break;
					} default:
					{
						system("cls");
						wcout << L"Nhập lại! Bạn nhập sai rồi !";
						break;
					}
					}
				}
				returnBack();
			} break; }
			case 3: {
				system("cls");
				while (true) {
					wcout << L"\n\n\t\t =========== CHÈN THEO TIÊU CHÍ ===========";
					wcout << L"\n1. Theo ngày tháng năm sinh";
					wcout << L"\n2. Theo chức vụ";
					wcout << L"\n3. Theo hệ số lương";
					wcout << L"\n0. Quay về màn hình chính!";
					wcout << L"\n\n\t\t =========== END ===========";
					while (true) {
						int choose; wcout << L"\nNhập lựa chọn: ";
						wcin >> choose;
						switch (choose)
						{
						case 0: { goto menuMain; }
						case 1: {
							system("cls");
							createNhanVien(l);
							sortByDate(l);
							outputListNhanVien(l);
							system("pause");
							wcout << L"Quay lại màn hình chính để kiểm tra !Nhấn phím bất kì!";
							goto menuMain; break; }
						case 2:
						{
							system("cls");
							createNhanVien(l);
							sortByPosition(l);
							outputListNhanVien(l);
							wcout << L"Quay lại màn hình chính để kiểm tra !Nhấn phím bất kì!";
							system("pause");
							goto menuMain;
							break; }
						case 3: {
							system("cls");
							createNhanVien(l);
							sortBySalary(l);
							outputListNhanVien(l);
							wcout << L"Quay lại màn hình chính để kiểm tra !Nhấn phím bất kì!";
							system("pause");
							goto menuMain; break;
						}
						default: {
							system("cls");
							wcout << L"Nhập lại! Bạn nhập sai rồi !";
							system("pause"); break; }
						}
					} returnBack();
				}
				break;
			}
			case 4:
			{
				deleteNhanVien(l);
				outputListNhanVien(l);
				system("pause");
				break;
			}
			case 5:
			{
				findNhanVien(l);
				system("pause");
				break;
			}
			case 6:
			{
				wofstream fileout("NHANVIEN.txt");
				locale utf8_locale(locale(), new codecvt_utf8<wchar_t>);
				fileout.imbue(utf8_locale);
				for (NODE *k = l.pHead; k != NULL; k = k->next)
				{
					if (k->next == NULL) {
						writeFile(fileout, k->data);
					}
					else {
						writeFile(fileout, k->data); fileout << endl;
					}
				}
				fileout.close();
				wcout << L"Xuất file thành công!";
				system("pause");
				break;
			} case 7:
			{ outputListNhanVien(l);
			wcout << endl;
			system("pause");
			break;
			}
			default: break;
			}
		}
	}

}



int main() {
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stdout), _O_U16TEXT);
	option();

	system("pause");
	return 0;
}
