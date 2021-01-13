#include "spreadsheet_data.hpp"

#include <algorithm>

using namespace std;

//Utilities
wstring alphabet_label(size_t index){
	wstring output;
	do{
		output.push_back(L'A' + static_cast<wchar_t>(index % 26));
		index /= 26;
	} while(index != 0);
	reverse(output.begin(),output.end());
	return output;
}

inline void clip_left_justify(wstring& str, const size_t w){
  str.erase(str.begin()+w,str.end());
}
inline void clip_right_justify(wstring& str, const size_t w){
  str.erase(str.begin(),str.begin()+(str.size()-w));
}
inline void clip_center_justify(wstring& str, const size_t w){
  size_t the_one = w%2, the_half = w>>1;
  str.erase(str.begin()+(str.size()-the_half-the_one),str.end());
  str.erase(str.begin(),str.begin()+the_half);
}
inline void extend_left_justify(wstring& str, const size_t w){
  str.resize(w,L' ');
}
inline void extend_right_justify(wstring& str, const size_t w){
  wstring output(w-str.size(),L' ');
  output += str;
  swap(output,str);
}
inline void extend_center_justify(wstring& str, const size_t w){
  const size_t space_width = w-str.size(), half_space_width = space_width>>1;
  wstring output(half_space_width,L' ');
  output += str;
  output.resize(output.size()+half_space_width+(space_width%2),L' ');
  swap(output,str);
}
void adjust_string(wstring& str, const size_t w, Justification just){
  if(str.size() > w){
    if(just == Left){
      clip_left_justify(str,w);
    } else if(just == Right){
      clip_right_justify(str,w);
    } else {
      clip_center_justify(str,w);
    }
  } else if(str.size() < w){
    if(just == Left){
      extend_left_justify(str,w);
    } else if(just == Right){
      extend_right_justify(str,w);
    } else {
      extend_center_justify(str,w);
    }
  }
}

//String Retrieval
wstring spreadsheet_data::get_abbr_entry_str(size_t r, size_t c) const{
  const data_entry& de = read_entry(r,c);
  wstring ret = de.str();
	adjust_string(ret,get_col_width(c),de.get_just());
  return ret;
}
wstring spreadsheet_data::get_entry_str(size_t r, size_t c) const{
  const data_entry& de = read_entry(r,c);
  return de.str();
}

wstring spreadsheet_data::get_row_label(const size_t row){
  wstring ret = to_wstring(row);
  adjust_string(ret,DEFAULT_WIDTH,Right);
  return ret;
}
wstring spreadsheet_data::get_col_label(const size_t col){
  wstring ret = alphabet_label(col);
  adjust_string(ret,get_col_width(col),Right);
  return ret;
}
//Entry Reading/Writing
const data_entry& spreadsheet_data::read_entry(const size_t r,
  const size_t c) const {
  if(entry_exists(r,c)){
    const size_t eid = get_entry_id(r,c);
    if(eid != npos){
      return entries[eid];
    }
  }
  const static data_entry blank_entry = data_entry();
  return blank_entry;
}
void spreadsheet_data::write_entry(const size_t r, const size_t c,
  const wstring& str){
  while(!col_exists(c)){
    size_t new_col_id = get_new_column_id();
    column_order.push_back(new_col_id);
  }
  if(!row_col_exists(r,c)){
    get_col(c).ids.resize(r+1,npos);
  }
  size_t &eid = get_entry_id(r,c);
  if(eid == npos){
    eid = get_new_entry_id();
  }
  entries[eid] = str;

  wstring out_str = entries[eid].str();
  adjust_string(out_str,get_col_width(c),entries[eid].get_just());
  view.update(r,c,out_str);
}
