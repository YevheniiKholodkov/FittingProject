#ifndef NLSFINIPRASER
#define NLSFINIPRASER
class NLSFIniPraser
{
public:
	NLSFIniPraser() {}
	
	bool readIni(string pathToIni);
	bool getCategories(vector<string> &categories);
	bool getFunctions(string category, vector<string>& functions);
private:
	Tree mTreeIni;
};

#endif //NLSFINIPRASER