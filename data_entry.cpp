#include "data_entry.hpp"

using namespace std;

//Constructors
data_entry::data_entry(const wstring& input){
  if(input == L""){
    contents.int_data = 0;
    type = Blank;
  } else try {
    long long int num = stoll(input);
    contents.int_data = num;
    type = Int;
  } catch(...){ try {
    size_t num = stoull(input);
    contents.unsigned_int_data = num;
    type = Unsigned;
  } catch(...){ try {
    double num = stod(input);
    contents.floating_data = num;
    type = Floating;
  } catch(...) {
    create_string(input);
    type = String;
  }}}
}

data_entry::data_entry(const data_entry& de){
  type = de.get_type();
  just = de.just;
  if(de.get_type() == String){
    create_string(de.get_string());
  } else {
    contents = de.contents;
  }
}

data_entry::~data_entry(){
  clear_string();
}

//Operators
data_entry& data_entry::operator=(const data_entry& de){
  if(de.get_type() == String){
    if(type == String){
      get_string() = de.get_string();
    } else {
      create_string(de.get_string());
    }
  } else {
    contents = de.contents;
  }
  type = de.get_type();
  just = de.just;
  return *this;
}

data_entry& data_entry::operator=(data_entry&& de){
  clear_string();
  contents = de.contents;
  type = de.type;
  just = de.just;
  de.type = Int; // Do not destroy string;
  return *this;
}

data_entry& data_entry::operator=(const wstring& input){
  return *this = data_entry(input);
}

//Type Stuff
void data_entry::switch_type(Entry_Type new_type){
  if(type == new_type) return;
  clear_string();
  switch(new_type){
    case String:
      create_string(L"");
      break;
    case Int:
      contents.int_data = 0;
      break;
    case Unsigned:
      contents.unsigned_int_data = 0;
      break;
    case Floating:
      contents.floating_data = 0;
      break;
    default:
      break;
  }
  type = new_type;
}

//Data Input
void data_entry::user_input_data(const wstring& input){
  if(type != String){
    type = String;
    create_string(input);
  } else {
    get_string() = input;
  }
}

void data_entry::user_input_data(const long long int input){
  if(type == Unsigned){
    if(input < 0){
      type = Int;
      contents.int_data = input;
    } else {
      contents.unsigned_int_data = static_cast<size_t>(input);
    }
  } else {
    clear_string();
    contents.int_data = input;
    type = Int;
  }
}

void data_entry::user_input_data(double input){
  clear_string();
  contents.floating_data = input;
  type = Int;
}

//String Conversion
wstring data_entry::str() const {
	switch(type){
		case Blank: return L"";
		case String: return get_string();
		case Int:	return to_wstring(contents.int_data);
		case Unsigned: return to_wstring(contents.unsigned_int_data);
		case Floating: return to_wstring(contents.floating_data);
	}
	return L"";
}
