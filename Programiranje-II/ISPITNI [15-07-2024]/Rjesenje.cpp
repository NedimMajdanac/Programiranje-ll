#include <iostream>
#include <mutex>
#include <regex>
#include <thread>
#include <vector>
#include <string>


using namespace std;

enum Drzava { ENGLESKA, SPANIJA, HOLANDIJA, FRANCUSKA, BOSNA_I_HERCEGOVINA };

ostream& operator<<(ostream& COUT, Drzava obj) {
	const char* ispis[] = { "ENGLESKA", "SPANIJA", "HOLANDIJA", "FRANCUSKA", "BOSNA_I_HERCEGOVINA" };
	COUT << ispis[obj];
	return COUT;
}
char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}
char* GenerisiID(int broj) {
	string id = "ID#";

	if (broj < 10)
		id = id + "000-" + to_string(broj);
	else if (broj>9&&broj < 100)
		id = id + "00-" + to_string(broj);
	else if (broj > 99 && broj < 1000)
		id = id + "0-" + to_string(broj);
	else 
		id = id + "-" + to_string(broj);

	return GetNizKaraktera(id.c_str());
}

bool ValidirajID(string id) {
	return regex_match(id, regex("(ID#)[0]{0,3}[-][0-9]{1,4}"));
}

const char* crt = "\n------------------------------\n";

template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int* _trenutno;
public:
	Kolekcija() {
		_trenutno = new int(0);
	}
	Kolekcija(const Kolekcija& obj)
	{
		_trenutno = new int(*obj._trenutno);
		for (size_t i = 0; i < getTrenutno(); i++)
		{
			_elementi1[i] = new T1(*obj._elementi1[i]);
			_elementi2[i] = new T2(*obj._elementi2[i]);
		}
	}
	Kolekcija& operator=(const Kolekcija& obj)
	{
		if(this!=&obj){
			for (size_t i = 0; i < *_trenutno; i++)
			{
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}
			delete _trenutno; _trenutno = nullptr;
			_trenutno = new int(*obj._trenutno);
			for (size_t i = 0; i < getTrenutno(); i++)
			{
				_elementi1[i] = new T1(*obj._elementi1[i]);
				_elementi2[i] = new T2(*obj._elementi2[i]);
			}
		}
		return *this;
	}

	~Kolekcija() {
		for (size_t i = 0; i < *_trenutno; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
	}
	T1& getElement1(int lokacija)const {
		return *_elementi1[lokacija];
	}
	T2& getElement2(int lokacija)const {
		return *_elementi2[lokacija];
	}
	int getTrenutno() const { return *_trenutno; }
	friend ostream& operator<< (ostream& COUT, Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) <<
			endl;
		return COUT;
	}
	void AddElement(T1 el1, T2 el2) {
		_elementi1[getTrenutno()] = new T1(el1);
		_elementi2[getTrenutno()] = new T2(el2);
		(*_trenutno)++;
	}
	Kolekcija InsertAt(int lokacija, T1 el1, T2 el2) {
		for (size_t i = getTrenutno(); i > lokacija; i--)
		{
			_elementi1[i] = _elementi1[i - 1];
			_elementi2[i] = _elementi2[i - 1];
		}
		_elementi1[lokacija] = new T1(el1);
		_elementi2[lokacija] = new T2(el2);
		(*_trenutno)++;
		return *this;
	}
	void RemoveAt(int lokacija) {
		delete _elementi1[lokacija];
		delete _elementi2[lokacija];
		(*_trenutno)--;
		for (size_t i = lokacija; i < getTrenutno(); i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
	}
	Kolekcija* RemoveRange(int from, int to) {
		if (from<0 || to>getTrenutno())
			throw exception("VAN DOSEGA\n");
		Kolekcija<T1, T2, max>* temp = new Kolekcija();
		for (size_t i = from; i <= to; i++)
		{
			temp->AddElement(*_elementi1[i], *_elementi2[i]);
		}
		for (size_t i = to; i >= from; i--)
		{
			RemoveAt(i);
		}
		return temp;
	}
};

class Vrijeme {
	int* _sat, * _minuta, * _sekunda;
public:
	Vrijeme(int sat = 10, int minuta = 0, int sekunda = 0) {
		_sat = new int(sat);
		_minuta = new int(minuta);
		_sekunda = new int(sekunda);
	}
	Vrijeme(const Vrijeme& obj)
	{
		_sat = new int(*obj._sat);
		_minuta = new int(*obj._minuta);
		_sekunda = new int(*obj._sekunda);
	}
	Vrijeme& operator=(const Vrijeme& obj)
	{
		if(this!=&obj){
			delete _sat; _sat = nullptr;
			delete _minuta; _minuta = nullptr;
			delete _sekunda; _sekunda = nullptr;
			_sat = new int(*obj._sat);
			_minuta = new int(*obj._minuta);
			_sekunda = new int(*obj._sekunda);
		}
		return *this;
	}
	~Vrijeme() {
		delete _sat; _sat = nullptr;
		delete _minuta; _minuta = nullptr;
		delete _sekunda; _sekunda = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, const Vrijeme& obj) {
		COUT << *obj._sat << ":" << *obj._minuta << ":" <<
			*obj._sekunda;
		return COUT;
	}

	int toSecs() {
		return *_sat * 3600 + *_minuta * 60 + *_sekunda;
	}
	bool operator==(Vrijeme& obj) {
		return this->toSecs() == obj.toSecs();
	}
};
class Pogodak {
	Vrijeme _vrijemePogotka;
	char* _napomena;
public:
	Pogodak(Vrijeme vrijeme, const char* napomena) :
		_vrijemePogotka(vrijeme)
	{
		_napomena = GetNizKaraktera(napomena);
	}
	Pogodak(const Pogodak& obj)
	{
		_vrijemePogotka = obj._vrijemePogotka;
		_napomena = GetNizKaraktera(obj._napomena);
	}
	Pogodak& operator=(const Pogodak& obj)
	{
		if(this!=&obj){
			delete[] _napomena; _napomena = nullptr;
			_vrijemePogotka = obj._vrijemePogotka;
			_napomena = GetNizKaraktera(obj._napomena);
		}
		return *this;
	}
	~Pogodak()
	{
		delete[] _napomena; _napomena = nullptr;
	}
	Vrijeme GetVrijemePogotka() { return _vrijemePogotka; }
	char* GetNapomena() { return _napomena; }
	friend ostream& operator<< (ostream& COUT, const Pogodak& obj) {
		COUT << obj._vrijemePogotka << " -> " << obj._napomena;
		return COUT;
	}

	bool operator==(Pogodak& obj) {
		return _vrijemePogotka == obj._vrijemePogotka;
	}

};
class Igrac {
	static int _id;
	char* _ID; // za inicijalizaciju _ID-a iskoristiti funkciju GenerisiID i vrijednost statickog clana _id
	char* _imePrezime;
	vector<Pogodak> _pogoci;
public:
	Igrac(const char* imePrezime)
	{
		_imePrezime = GetNizKaraktera(imePrezime);
		_ID = GenerisiID(_id++);
	}
	Igrac(const Igrac& obj)
	{
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_ID = GetNizKaraktera(obj._ID);
		_pogoci = obj._pogoci;
	}
	Igrac& operator=(const Igrac& obj)
	{
		if(this!=&obj){
			delete[]_ID; _ID = nullptr;
			delete[]_imePrezime; _imePrezime = nullptr;
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_ID = GetNizKaraktera(obj._ID);
			_pogoci = obj._pogoci;
		}
		return *this;
	}
	~Igrac()
	{
		delete[]_ID; _ID = nullptr;
		delete[]_imePrezime; _imePrezime = nullptr;
	}
	char* GetImePrezime() { return _imePrezime; }
	char* GetID() { return _ID; }
	vector<Pogodak>& GetPogoci() { return _pogoci; }
	friend ostream& operator<< (ostream& COUT, Igrac& obj) {
		COUT << *obj._ID << " -> " << obj._imePrezime;
		for (size_t i = 0; i < obj._pogoci.size(); i++)
			cout << obj._pogoci[i] << endl;
		return COUT;
	}

	bool operator==(Igrac& obj) {
		return strcmp(_ID, obj._ID) == 0;
	}

	bool checkPlayerPogodak(Pogodak pogodak) {
		if (!_pogoci.empty()) {
			for (size_t i = 0; i < _pogoci.size(); i++)
			{
				if (_pogoci[i] == pogodak)
					return true;
			}
		}
		return false;
	}
};

int Igrac::_id = 1;
class Reprezentacija {
	Drzava _drzava;
	vector<Igrac> _igraci;
public:
	Reprezentacija(Drzava drzava) {
		_drzava = drzava;
	}
	Reprezentacija(const Reprezentacija& obj)
	{
		_drzava = obj._drzava;
		_igraci = obj._igraci;
	}
	Reprezentacija& operator=(const Reprezentacija& obj)
	{
		if (this != &obj) {
			_drzava = obj._drzava;
			_igraci = obj._igraci;
		}
		return *this;
	}
	Drzava GetDrzava() { return _drzava; }
	vector<Igrac>& GetIgraci() { return _igraci; }

	void AddIgrac(Igrac player) {
		for (size_t i = 0; i < _igraci.size(); i++)
		{
			if (_igraci[i] == player)
				throw exception("IGRAC SA TIM ID JE EVIDENTIRAN\n");
		}
		_igraci.push_back(player);
	}

	bool operator==(Reprezentacija& obj) {
		return _drzava == obj._drzava;
	}

	Igrac* getByIDorIME(const char* idORime) {
		for (size_t i = 0; i < _igraci.size(); i++)
		{
			if (strcmp(_igraci[i].GetID(), idORime) == 0 || strcmp(_igraci[i].GetImePrezime(), idORime) == 0)
				return &_igraci[i];
		}
		return nullptr;
	}

	int getBrojPogodaka() {
		int brojPogodaka = 0;
		for (size_t i = 0; i < _igraci.size(); i++)
		{
			brojPogodaka += _igraci[i].GetPogoci().size();
		}
		return brojPogodaka;
	}

	vector<Igrac> getScorers() {
		vector<Igrac> IgraciSaPogotkom;
		for (size_t i = 0; i < _igraci.size(); i++)
		{
			if (!_igraci[i].GetPogoci().empty()) {
				for (size_t j = 0; j < _igraci[i].GetPogoci().size(); j++)
				{
					IgraciSaPogotkom.push_back(_igraci[i]);
				}
			}
		}
		return IgraciSaPogotkom;
	}

	~Reprezentacija(){} 
};

mutex m;
class Prventstvo {
	Kolekcija<Reprezentacija, Reprezentacija, 20> _utakmice;
	void sendMail(Igrac igrac, int pogodak,Vrijeme vrijeme, Reprezentacija repka1, Reprezentacija repka2) {
		m.lock();
		cout << "To: " << igrac.GetID() << "euro2024.com" << endl;
		cout << "From: info@euro2024.com\nSubject: Informacija" << endl;
		cout << "Postovani, " << endl;
		cout << "U " << vrijeme << " sati igrac " << igrac.GetImePrezime() << " je zabiljezio svoj " << pogodak << " pogodak na ovoj utakmici. " << endl;
		cout << "Trenutni rezultat je : " << endl;
		cout << repka1.GetDrzava() << " " << repka1.getBrojPogodaka() << " : " << repka2.getBrojPogodaka() << " " << repka2.GetDrzava() << endl;
		cout << " Puno srece u nastavku susreta.\nNeka bolji tim pobijedi." << crt;
		m.unlock();
	}
public:
	Prventstvo()
	{

	}
	Prventstvo(const Prventstvo& obj)
	{
		_utakmice = obj._utakmice;
	}
	Prventstvo& operator=(const Prventstvo& obj)
	{
		if (this != &obj)
			_utakmice = obj._utakmice;
		return *this;
	}
	Kolekcija<Reprezentacija, Reprezentacija, 20>& GetUtakmice() {
		return _utakmice;
	}

	void AddUtakmicu(Reprezentacija repka1, Reprezentacija repka2) {
		for (size_t i = 0; i < _utakmice.getTrenutno(); i++)
		{
			if ((_utakmice.getElement1(i) == repka1 && _utakmice.getElement2(i) == repka2) ||
				(_utakmice.getElement1(i) == repka2 && _utakmice.getElement2(i) == repka1))
				throw exception("UTAKMICA JE VEC ODIGRANA\n");
		}
		_utakmice.AddElement(repka1, repka2);
	}

	bool checkPogodak(Pogodak pogodak) {
		for (size_t i = 0; i < _utakmice.getTrenutno(); i++)
		{
			for (size_t j = 0; j < _utakmice.getElement1(i).GetIgraci().size(); j++)
				if (_utakmice.getElement1(i).GetIgraci()[j].checkPlayerPogodak(pogodak))
					return true;
			for (size_t j = 0; j < _utakmice.getElement2(i).GetIgraci().size(); j++)
				if (_utakmice.getElement2(i).GetIgraci()[j].checkPlayerPogodak(pogodak))
					return true;		
		}
		return false;
	}

	bool AddPogodak(Drzava drz1, Drzava drz2, const char* IDorIme, Pogodak pogodak) {
		for (size_t i = 0; i < _utakmice.getTrenutno(); i++)
		{
			Reprezentacija& repka1 = _utakmice.getElement1(i);
			Reprezentacija& repka2 = _utakmice.getElement2(i);

			if (repka1.GetDrzava() == drz1 && repka2.GetDrzava() == drz2 ||
				repka1.GetDrzava() == drz2 && repka2.GetDrzava() == drz1) {

				Igrac* igrac = repka1.getByIDorIME(IDorIme);
				if (!igrac) { repka2.getByIDorIME(IDorIme); }

				if (igrac && !checkPogodak(pogodak)) {
					igrac->GetPogoci().push_back(pogodak);
					this_thread::sleep_for(2s);
					thread t1(&Prventstvo::sendMail, this, *igrac, igrac->GetPogoci().size(), pogodak.GetVrijemePogotka(), repka1, repka2);
					t1.join();
					return true;
				}
			}
		}
		return false;
	}
	//-------------------------------------------
	//BOSNA_I_HERCEGOVINA 3 : 1 ENGLESKA
	//-------------------------------------------
	//Denis Music Goran Skondric
	//Jasmin Azemovic
	//Jasmin Azemovic
	//-------------------------------------------

	friend ostream& operator<<(ostream& COUT, Prventstvo& obj) {
		for (size_t i = 0; i < obj._utakmice.getTrenutno(); i++)
		{
			auto& repka1 = obj._utakmice.getElement1(i);
			auto& repka2 = obj._utakmice.getElement2(i);
			auto scorers1 = repka1.getScorers();
			auto scorers2 = repka2.getScorers();
			COUT << crt;
			COUT << repka1.GetDrzava() << " " << repka1.getBrojPogodaka() << " : " << repka2.getBrojPogodaka() << " " << repka2.GetDrzava() << endl;
			COUT << crt;
			int maxSIZE = max(scorers1.size(), scorers2.size());
			for (size_t j = 0; j < maxSIZE; j++)
			{
				if (j < scorers1.size())
					COUT << scorers1[j].GetImePrezime() << endl;
				if (j < scorers2.size())
					COUT << scorers2[j].GetImePrezime() << endl;
			}
			COUT << crt;
		}
		return COUT;
	}

	~Prventstvo() {}
};

void main() {

	///*
	//Globalna funkcija GenerisiID vraca ID igraca (format: ID#00-ID) na
 //  osnovu int vrijednosti proslijedjene
	//kao parametar. Funkcija generise ID sa maksimalno 4 cifre, ne
 //  racunajuci ostale, podrazumijevane, znakove.
	//Podrazumijeva se da ce uvijek biti proslijedjena validna int
 //  vrijednost.
	//*/
	cout << GenerisiID(3) << endl;//treba vratiti ID#000-3
	cout << GenerisiID(14) << endl;//treba vratiti ID#00-14
	cout << GenerisiID(156) << endl;//treba vratiti ID#0-156
	cout << GenerisiID(1798) << endl;//treba vratiti ID#1798

	//Za validaciju ID-a koristiti funkciju ValidirajID koja treba	koristeci regex, osigurati postivanje osnovnih pravila
		//vezanih za format koja su definisana u prethodnom dijelu zadatka.
	if (ValidirajID("ID#000-3"))
		cout << "ID VALIDAN" << endl;
	if (ValidirajID("ID#0-156"))
		cout << "ID VALIDAN" << endl;
	if (!ValidirajID("ID#120-3"))
		cout << "ID NIJE VALIDAN" << endl;
	if (!ValidirajID("ID#00-02"))
		cout << "ID NIJE VALIDAN" << endl;
	
	int kolekcijaTestSize = 9;
	Kolekcija<int, int, 10> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);//dodaje vrijednosti u kolekciju 
	cout << kolekcija1 << crt;
	
	
	/* metoda InsertAt treba da doda vrijednosti drugog i treceg
   parametra na lokaciju koja je definisana prvim parametrom. Povratna
   vrijednost metode
	je objekat (pozivaoc metode, u konkretnom slucaju objekat
   kolekcija1) u okviru koga su, na definisanu lokaciju, dodati
   zahtijevani parametri.
	Nakon izvrsenja metode InsertAt, oba objekta, kolekcija1 i
   kolekcija2, bi trebali posjedovati sljedeci sadrzaj:
	10 10
	0 0
	1 1
	2 2
	* ....
	*/
	Kolekcija<int, int, 10> kolekcija2 = kolekcija1.InsertAt(0, 10, 10);
	cout << kolekcija2 << crt;
	
	/*Metoda RemoveRange prihvata lokacija OD i DO, te u tom opsegu
   uklanja sve elemente iz kolekcije. U slucaju da zahtijevani opseg ne
   postoji u kolekciji
	metoda treba baciti izuzetak. Na kraju, metoda treba da vrati
   pokazivac na novi objekat tipa kolekcija koji sadrzi samo uklonjene
   elemente*/
	Kolekcija<int, int, 10>* kolekcija3 = kolekcija1.RemoveRange(1, 3);
	cout << *kolekcija3 << endl;
	cout << kolekcija1 << crt;
	/*kolekcija3 bi trebala sadrzavati sljedece elemente:
	0 0
	1 1
	2 2
	dok bi kolekcija1 trebala sadrzavati sljedece elemente:
	10 10
	3 3
	4 4
	......
	*/
	kolekcija1 = *kolekcija3;
	cout << kolekcija1;
	
	
	Vrijeme
		prviPogodak201633(20, 16, 33),
		drugiPogodak202319(20, 23, 19),
		treciPogodak205108(20, 51, 8),
		cetvrtiPogodak210654(21, 6, 54);
	
	Igrac denis("Denis Music"), jasmin("Jasmin Azemovic"),
		goran("Goran Skondric"), adil("Adil Joldic");
	
	if (strcmp(denis.GetID(), "ID#000-1") == 0 &&
		strcmp(jasmin.GetID(), "ID#000-2") == 0)
		cout << "ID se uspjesno generise!" << endl;
	
	Pogodak prviPogodak(prviPogodak201633, "podaci o prvom pogotku"),
		drugiPogodak(drugiPogodak202319, "podaci o drugom pogotku"),
		treciPogodak(treciPogodak205108, "podaci o trecem pogotku"),
		cetvrtiPogodak(cetvrtiPogodak210654, "podaci o cetvrtom pogotku");
	
	Reprezentacija BIH(BOSNA_I_HERCEGOVINA), ENG(ENGLESKA);
	BIH.AddIgrac(denis);
	BIH.AddIgrac(jasmin);
	ENG.AddIgrac(goran);
	ENG.AddIgrac(adil);
	try
	{
		//onemoguciti dodavanje istih igraca - provjeravati ID, baciti izuzetak
			BIH.AddIgrac(denis);
	}
	catch (exception& obj)
	{
		cout << obj.what();
	}
	
	Prventstvo euro2024;
	
	euro2024.AddUtakmicu(BIH, ENG);
	
	try
	{
		//onemoguciti ponovne susrete drzave tokom istog prvenstva,baciti izuzetak
		euro2024.AddUtakmicu(BIH, ENG);
	}
	catch (exception& obj)
	{
		cout << obj.what();
	}
	
	//omoguciti dodavanje pogotka po ID-u ili imenu i prezimenu
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-1",
		prviPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	//onemoguciti dodavanje istih pogodaka
	if (!euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Denis Music", prviPogodak))
		cout << "Pogodak NIJE uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "ID#000-2", drugiPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Jasmin Azemovic", treciPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	if (euro2024.AddPogodak(BOSNA_I_HERCEGOVINA, ENGLESKA, "Goran Skondric", cetvrtiPogodak))
		cout << "Pogodak uspjesno dodat" << endl;
	
	////nakon svakog evidentiranog pogotka, svim igracima te utakmice (pod pretpostavkom da su validne email adrese sa ID - ovima igraca),
	//	//u zasebnom thread-u, poslati email, u razmaku od 2 sekunde, sa sljedecim sadrzajem :
	///*
	//To: ID#000-1@euro2024.com
	//From: info@euro2024.com
	//Subject: Informacija
	//Postovani,
	//U 20:35:16 sati igrac Jasmin Azemovic je zabiljezio svoj 1
 //  pogodak na ovoj utakmici.
	//Trenutni rezultat je:
	//BOSNA_I_HERCEGOVINA 2 : 0 ENGLESKA
	//Puno srece u nastavku susreta.
	//Neka bolji tim pobijedi.
	
	//*/
	////ispisuje detaljnije informacije o susretu, kako je navedeno unarednom ispisu
	cout << euro2024;
	///*
	//-------------------------------------------
	//BOSNA_I_HERCEGOVINA 3 : 1 ENGLESKA
	//-------------------------------------------
	//Denis Music Goran Skondric
	//Jasmin Azemovic
	//Jasmin Azemovic
	//-------------------------------------------
	//*/
	
	////vraca sve igrace koji su na takmicenju postigli pogodak u vremenu koje se nalazi izmedju proslijedjenih vrijednosti
	//vector<Igrac*> igraci = euro2024(Vrijeme(20, 15, 15), Vrijeme(20,
	//	24, 15));
	//for (size_t i = 0; i < igraci.size(); i++)
	//	cout << igraci[i]->GetImePrezime() << endl;
	cin.get();
	system("pause>0");
}