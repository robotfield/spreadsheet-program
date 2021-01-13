#ifndef PERSPECTIVE_HPP
#define PERSPECTIVE_HPP

#include <deque>
#include <string>
#include <iostream>
#include <cstddef>

class perspective {
public:
    perspective() {}

    //Shifts perspective to (row,col) with dims widthxheight
    void shift_perspective(size_t,size_t,size_t,size_t);

    inline void refresh(){ shift_perspective(anchor_r,anchor_c,height,width); }
    inline void resize(size_t h, size_t w){
      shift_perspective(anchor_r,anchor_c,h,w);
    }

    void shift_up(const std::wstring&);
    void shift_left(const std::wstring&);
    void shift_down(const std::wstring&);
    void shift_right(const std::wstring&);

    void fill(const std::wstring&);

    inline void fill_col_data(const std::wstring& str){
      if(col_data_unfilled()){
        col_data.push_back(DimLabel{str});
      }
    }
    inline void fill_row_data(const std::wstring& str){
      if(row_data_unfilled()){
        row_data.push_back(DimLabel{str});
      }
    }

    inline bool row_data_unfilled() const {
      return row_data.size() < height;
    }
    inline bool col_data_unfilled() const {
      return col_data.size() < width;
    }
    inline size_t next_row_data() const {
      return anchor_r + row_data.size();
    }
    inline size_t next_col_data() const {
      return anchor_c + col_data.size();
    }

    inline bool contains(size_t r, size_t c) const {
      return anchor_r <= r && anchor_c <= c
             && r < anchor_r + height && c < anchor_c + width;
    }

    inline size_t absolute_c(size_t c) const {
      return c + anchor_c;
    }

    inline size_t absolute_r(size_t r) const {
      return r + anchor_r;
    }

    inline size_t top_row_num() const {
      return anchor_r;
    }
    inline size_t bottom_row_num() const {
      return anchor_r + height - 1;
    }
    inline size_t left_col_num() const {
      return anchor_c;
    }
    inline size_t right_col_num() const {
      return anchor_c + width - 1;
    }

    inline bool unfilled() const {
      return filled != 0;
    }

    inline size_t next_row() const { return absolute_r(filled_row); }
    inline size_t next_col() const { return absolute_c(filled_col); }

    inline void update(size_t abs_r, size_t abs_c, const std::wstring& str){
      if(contains(abs_r,abs_c)){
        entry_strings[abs_r-anchor_r][abs_c-anchor_c] = str;
      }
    }

    void pretty_print(std::wostream& out) const;

private:
    struct DimLabel {
      std::wstring label;
    };
    std::deque<DimLabel> col_data;
    std::deque<DimLabel> row_data;

    std::deque<std::wstring> blank_row;
    std::deque<std::deque<std::wstring>> entry_strings;

    size_t width = 0, height = 0,
           anchor_r = 0, anchor_c = 0,
           filled_row = 0, filled_col = 0;

    int filled = 0;

    void insert_singlet_left(const std::wstring&);
    void insert_singlet_right(const std::wstring&);
    void insert_singlet_bottom(const std::wstring&);
    void insert_singlet_top(const std::wstring&);
    void fill_singlet(const std::wstring&);

    void init_blank_row_bottom();
    void init_blank_row_top();

};

#endif
