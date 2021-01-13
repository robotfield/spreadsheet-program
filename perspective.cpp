#include "perspective.hpp"

using namespace std;

static constexpr wchar_t BOX_UPPER_RIGHT = L'┌';
static constexpr wchar_t BOX_UPPER_LEFT = L'┐';
static constexpr wchar_t BOX_LOWER_RIGHT = L'└';
static constexpr wchar_t BOX_LOWER_LEFT = L'┘';
static constexpr wchar_t BOX_VERT = L'│';
static constexpr wchar_t BOX_VERT_CONNECT_RIGHT = L'├';
static constexpr wchar_t BOX_VERT_CONNECT_LEFT = L'┤';
static constexpr wchar_t BOX_VERT_CONNECT_ALL = L'┼';
static constexpr wchar_t BOX_HORIZ = L'─';
static constexpr wchar_t BOX_HORIZ_CONNECT_BOTTOM = L'┬';
static constexpr wchar_t BOX_HORIZ_CONNECT_TOP = L'┴';

//Printing
void perspective::pretty_print(wostream& out) const {
  if(height == 0 || width == 0) return;
  //Print top bar
  out << BOX_UPPER_RIGHT;
  for(size_t i = 0; i < row_data[0].label.size(); ++i){
    out << BOX_HORIZ;
  }
  for(const auto& str : entry_strings[0]){
    out << BOX_HORIZ_CONNECT_BOTTOM;
    for(size_t i = 0; i < str.size(); ++i){
      out << BOX_HORIZ;
    }
  }
  out << BOX_UPPER_LEFT << L'\n';

  //Print first row of cells
  out << BOX_VERT;
  for(size_t i = 0; i < row_data[0].label.size(); ++i){
    out << L' ';
  }
  out << BOX_VERT;
  for(const auto& col : col_data){
    out << col.label << BOX_VERT;
  }
  out << L'\n';

  for(size_t i = 0; i < height; ++i){
    //Print mid bar
    out << BOX_VERT_CONNECT_RIGHT;
    for(size_t i = 0; i < row_data[0].label.size(); ++i){
      out << BOX_HORIZ;
    }
    for(const auto& str : entry_strings[0]){
      out << BOX_VERT_CONNECT_ALL;
      for(size_t i = 0; i < str.size(); ++i){
        out << BOX_HORIZ;
      }
    }
    out << BOX_VERT_CONNECT_LEFT << L'\n';
    //Print strings
    out << BOX_VERT << row_data[i].label << BOX_VERT;
    for(const auto& str : entry_strings[i]){
      out << str << BOX_VERT;
    }
    out << L'\n';
  }

  //Print bottom bar
  out << BOX_LOWER_RIGHT;
  for(size_t i = 0; i < row_data[0].label.size(); ++i){
    out << BOX_HORIZ;
  }
  for(const auto& str : entry_strings[0]){
    out << BOX_HORIZ_CONNECT_TOP;
    for(size_t i = 0; i < str.size(); ++i){
      out << BOX_HORIZ;
    }
  }
  out << BOX_LOWER_LEFT << endl;
}

//Shifting
void perspective::shift_perspective(size_t new_anchor_r, size_t new_anchor_c,
                                    size_t new_height, size_t new_width){
  entry_strings.clear();
  anchor_r = new_anchor_r;
  anchor_c = new_anchor_c;
  height = new_height;
  width = new_width;
  filled = 5;

  blank_row.clear();
  blank_row.resize(width,L"");
  while(entry_strings.size() < height){
    entry_strings.push_back(blank_row);
  }

  row_data.clear();
  col_data.clear();
}

void perspective::shift_up(const wstring& label){
  if(filled == 0 && anchor_r != 0){
    filled = 1;
    init_blank_row_top();
    --anchor_r;
    row_data.pop_back();
    row_data.push_front(DimLabel{label});
  }
}

void perspective::shift_left(const wstring& label){
  if(filled == 0 && anchor_c != 0){
    filled = 2;
    --anchor_c;
    col_data.pop_back();
    col_data.push_front(DimLabel{label});
  }
}

void perspective::shift_down(const wstring& label){
  if(filled == 0){
    filled = 3;
    init_blank_row_bottom();
    ++anchor_r;
    row_data.pop_front();
    row_data.push_back(DimLabel{label});
  }
}

void perspective::shift_right(const wstring& label){
  if(filled == 0){
    filled = 4;
    ++anchor_c;
    col_data.pop_front();
    col_data.push_back(DimLabel{label});
  }
}

//Filling
void perspective::fill(const wstring& str){
  switch(filled){
    case 1:
      insert_singlet_top(str);
      break;
    case 2:
      insert_singlet_left(str);
      break;
    case 3:
      insert_singlet_bottom(str);
      break;
    case 4:
      insert_singlet_right(str);
      break;
    case 5:
      fill_singlet(str);
      break;
    default:
      break;
  }
}

void perspective::insert_singlet_left(const wstring& str){
  entry_strings[filled_row].pop_back();
  entry_strings[filled_row].push_front(str);
  ++filled_row;
  if(filled_row >= height){
    filled = 0;
    filled_row = 0;
  }
}

void perspective::insert_singlet_right(const wstring& str){
  entry_strings[filled_row].pop_front();
  entry_strings[filled_row].push_back(str);
  ++filled_row;
  if(filled_row >= height){
    filled = 0;
    filled_row = 0;
  }
}

void perspective::insert_singlet_bottom(const wstring& str){
  entry_strings.back()[filled_col] = str;
  ++filled_col;
  if(filled_col >= width){
    filled = 0;
    filled_col = 0;
  }
}

void perspective::insert_singlet_top(const wstring& str){
  entry_strings.front()[filled_col] = str;
  ++filled_col;
  if(filled_col >= width){
    filled = 0;
    filled_col = 0;
  }
}

void perspective::fill_singlet(const wstring& str){
  entry_strings[filled_row][filled_col] = str;
  ++filled_col;
  if(filled_col >= width){
    filled_col = 0;
    ++filled_row;
    if(filled_row >= height){
      filled = 0;
      filled_row = 0;
    }
  }
}

void perspective::init_blank_row_bottom(){
  if(blank_row.size() != width){
    blank_row.resize(width,L"");
  }
  entry_strings.pop_front();
  entry_strings.push_back(blank_row);
}

void perspective::init_blank_row_top(){
  if(blank_row.size() != width){
    blank_row.resize(width,L"");
  }
  entry_strings.pop_back();
  entry_strings.push_front(blank_row);
}
