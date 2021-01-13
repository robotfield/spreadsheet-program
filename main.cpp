#include <iostream>
#include <string>
#include <deque>

#include "spreadsheet_data.hpp"

using namespace std;

int main(){
	spreadsheet_data sd;
	sd.set_view(0,0,10,10);

	auto& view = sd.get_view();

	wstring input;

	while(true){
		view.pretty_print(wcout);
		wcin >> input;

		if(input == L"exit"){
			return 0;
		} else if(input == L"edit"){
			size_t r,c;
			wcin >> r >> c >> input;
			sd.modify_absolute(r,c,input);
		} else if(input == L"delete"){
			size_t r,c;
			wcin >> r >> c;
			sd.modify_absolute(r,c,L"");
		} else if(input == L"w"){
			sd.move_view_up();
		} else if(input == L"a"){
			sd.move_view_left();
		} else if(input == L"s"){
			sd.move_view_down();
		} else if(input == L"d"){
			sd.move_view_right();
		} else if(input == L"resize"){
			size_t w,h;
			wcin >> w >> h;
			sd.resize_view(w,h);
		}
	}

}
