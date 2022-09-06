#include "pch.h"
#include <memory>
#include "VString.h"


VString::VString(const char* dat) {

	int len = 0;
	for (int i = 0; i < 2555; i++) {

		if (dat[i] != "\0"[0])
		{
			len++;
		}
		else {
			break;
		}

	}

	data = (char*)malloc(len);

	for (int i = 0; i < len; i++) {
		data[i] = dat[i];
	}
	data[len] = "\0"[0];
	//data = (char*)dat;


}

VString::VString() {

	data = (char*)malloc(1);
	data[0] = "\0"[0];

}

int VString::Find(const char* f,int sc) {

	for (int i = sc; i < 2555; i++) {

		if (data[i] == f[0])
		{
			return i;
		}
		if (data[i] == "\0"[0])
		{
			return -1;
		}

	}
	return -1;

}

VString::VString(float f) {

//	while(true){}
	char* ar =(char*)malloc(30);

	sprintf(ar, "%f", f);
	data = ar;

	VString t1(data);

	int mp = t1.Find(".");

	data = (char*)VString(data).SubString(0, mp + 2).GetConst();


	
}

VString::VString(int f) {

	char* ar = (char*)malloc(30);

	sprintf(ar, "%d", f);
	data = ar;

}

VString VString::SubString(int i, int l) {
	if (i == 0 && l == Len()) {
		return VString(data);
	}
	if (l == -1) {
		l = Len() - i;
	}

	if (l == 0) return VString("");
	char* ns = (char*)malloc(l + 1);
	for (int k = 0; k < l; k++) {

		ns[k] = GetChar(i + k);

	}
	ns[l] = "\0"[0];
	return VString(ns);

}


int VString::ToInt() {

	return (int)atoi(data);
}

float VString::ToFloat() {

	float v = (float)atof(data);

	return v;
}


double VString::ToDouble() {

	double v = atof(data);
	return v;

}

void VString::Replace(char c,char w) {

	for (int i = 0; i < 2555; i++) {

		if (data[i] == "\0"[0])
		{
			break;
		}
		if (data[i] == c)
		{
			data[i] = w;
		}

	}

}

int VString::Len() {

	for (int i = 0; i < 2555; i++) {

	if (data[i] == "\0"[0])
		{
			return i;
		}

	}
	return -1;

}

void VString::Add(VString a) {

	int plen = Len();

	int nl = Len() + a.Len();

	char* ndat = (char*)malloc(nl + 1);

	for (int i = 0; i < Len(); i++) {
		ndat[i] = data[i];
	}

	for (int i = 0; i < a.Len(); i++) {

		ndat[plen + i] = a.GetChar(i);

	}
	ndat[plen + a.Len()] = "\0"[0];

	data = ndat;

}

char VString::GetChar(int n)
{

	return data[n];

}
