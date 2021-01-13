#ifndef DATA_ENTRY_HPP
#define DATA_ENTRY_HPP

#include <string>
#include <exception>
#include <cstring>
#include <cstddef>

enum Entry_Type : char {
			Blank = '\0',
			String,
			Int,
			Unsigned,
			Floating
};
enum Justification : char {
			Left, Right, Center
};

class data_entry {
public:
		data_entry() : type(Blank) {}
		data_entry(const std::wstring&);
		data_entry(const data_entry&);
		~data_entry();

		data_entry& operator=(const data_entry&);
		data_entry& operator=(data_entry&&);
		data_entry& operator=(const std::wstring&);

		void clear() { switch_type(Blank); }

		void switch_type(Entry_Type);

		void user_input_data(const std::wstring&);
		void user_input_data(const long long int);
		void user_input_data(double);

		std::wstring str() const;

		Entry_Type get_type() const { return type; }

		Justification get_just() const{ return (type == Blank) ? Left : just; }


private:
		union Contents {
			// Contents() : int_data(0) {}
			// ~Contents(){}
			// Contents& operator=(const Contents& other){
			// 	memcpy(this,&other,sizeof(Contents));
			// 	return *this;
			// }
			std::wstring* str_data;
			long long int int_data;
			size_t unsigned_int_data;
			double floating_data;
		} contents;

		Entry_Type type;
		Justification just = Right;

		inline void clear_string(){
			if(type == String){
				delete contents.str_data;
			}
		}
		inline std::wstring& get_string(){
			return *contents.str_data;
		}
		inline const std::wstring& get_string() const {
			return *contents.str_data;
		}
		inline void create_string(const std::wstring& str){
			contents.str_data = new std::wstring(str);
		}
};

#endif
