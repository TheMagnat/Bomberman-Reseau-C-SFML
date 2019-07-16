#include "StringToVec.hpp"
#include <iostream>
#include <cstdlib>

//Note : L'utilisation de cette fonction doit être faite avec une data PROPRE, c'est à dire que la partie qu'on veut traité se fini bien par '\0' et pas '\n' sinon la fonction risque de traiter de la data ancienne.
StringToVec::StringToVec(const char * data) : itte_(0) {
	char * next;

	next = strtok (const_cast<char *>(data)," ");

	while (next != NULL){
		if(*next == '\r' || *next == '\n' || *next == '\0') break;
		data_.push_back(atoi(next));
		next = strtok (NULL, " ");
	}

	size_ = data_.size();
}

int StringToVec::takeElement(){
	if(itte_ == size_) return -1;
	return data_[itte_++];
}