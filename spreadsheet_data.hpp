#ifndef SPREADSHEET_DATA_HPP
#define SPREADSHEET_DATA_HPP

#include <vector>
#include <stack>
#include <string>
#include <cstddef>

#include "data_entry.hpp"
#include "perspective.hpp"

static constexpr size_t DEFAULT_WIDTH = 6;
static constexpr size_t npos = static_cast<size_t>(-1);

class spreadsheet_data {
public:
	spreadsheet_data() {}

	inline void set_view(size_t ar, size_t ac, size_t vh, size_t vw){
		view.shift_perspective(ar,ac,vh,vw);
		fill_view();
	}

	inline void resize_view(size_t w, size_t h){
		view.resize(w,h);
		fill_view();
	}

	inline void refresh_view(){
		view.refresh();
		fill_view();
	}

	inline void move_view_up(){
		if(view.top_row_num() != 0){
			view.shift_up(get_row_label(view.top_row_num()-1));
			fill_view();
		}
	}

	inline void move_view_down(){
		view.shift_down(get_row_label(view.bottom_row_num()+1));
		fill_view();
	}

	inline void move_view_left(){
		if(view.left_col_num() != 0){
			view.shift_left(get_col_label(view.left_col_num()-1));
			fill_view();
		}
	}

	inline void move_view_right(){
		view.shift_right(get_col_label(view.right_col_num()+1));
		fill_view();
	}

	inline perspective& get_view(){
		return view;
	}

	inline void modify_absolute(size_t r, size_t c, const std::wstring& str){
		write_entry(r,c,str);
	}

	inline void modify(size_t r, size_t c, const std::wstring& str){
		modify_absolute(view.absolute_r(r),view.absolute_c(c),str);
	}

	std::wstring get_entry_str(size_t r, size_t c) const;
	std::wstring get_abbr_entry_str(size_t r, size_t c) const;

	void insert_column(size_t at){
		size_t col_id = get_new_column_id();
		column_order.insert(column_order.begin()+at,col_id);
	}
	void delete_column(size_t at){
		delete_column_id(column_order[at]);
		column_order.erase(column_order.begin()+at);
	}

	void insert_row(size_t at){
		row_heights.insert(row_heights.begin()+at,1);
		for(auto& col : columns){
			if(at < col.ids.size()){
				col.ids.insert(col.ids.begin()+at,npos);
			}
		}
	}
	void delete_row(size_t at){
		row_heights.erase(row_heights.begin()+at);
		for(auto& col : columns){
			if(at < col.ids.size()){
				delete_entry_id(col.ids[at]);
				col.ids.erase(col.ids.begin()+at);
			}
		}
	}

private:
	struct Column {
		std::vector<size_t> ids;
		size_t width = DEFAULT_WIDTH;
	};

	perspective view;

	std::vector<Column> columns;
	std::vector<data_entry> entries;
	std::vector<size_t> column_order;
	std::vector<size_t> row_heights;
	std::stack<size_t> unused_entry_ids;
	std::stack<size_t> unused_column_ids;

	//Private Functions
	inline bool col_exists(const size_t c) const {
		return c < column_order.size();
	}
	inline size_t get_col_width(const size_t c) const {
		return col_exists(c) ? get_col(c).width : DEFAULT_WIDTH;
	}

	inline bool row_col_exists(const size_t r, const size_t c) const {
		return r < get_col(c).ids.size();
	}

	inline bool entry_exists(const size_t r, const size_t c) const {
		return col_exists(c) && row_col_exists(r,c);
	}

	inline Column& get_col(const size_t c){ return columns[column_order[c]]; }
	inline const Column& get_col(const size_t c) const {
		return columns[column_order[c]];
	}

	inline size_t& get_entry_id(const size_t r, const size_t c) {
		return get_col(c).ids[r];
	}
	inline const size_t get_entry_id(const size_t r, const size_t c) const {
		return get_col(c).ids[r];
	}
	const data_entry& read_entry(const size_t r, const size_t c) const;
	void write_entry(const size_t r, const size_t c, const std::wstring&);

	std::wstring get_row_label(const size_t row);
	std::wstring get_col_label(const size_t col);

	size_t get_new_column_id(){
		size_t id;
		if(unused_column_ids.empty()){
			id = columns.size();
			columns.push_back(Column());
		} else {
			id = unused_column_ids.top();
			unused_column_ids.pop();
		}
		return id;
	}

	void delete_column_id(size_t id){
		Column& col = columns[id];
		for(auto entry_id : col.ids){
			delete_entry_id(entry_id);
		}
		col.ids.clear();
		col.width = DEFAULT_WIDTH;
		unused_column_ids.push(id);
	}
	size_t get_new_entry_id(){
		size_t id;
		if(unused_entry_ids.empty()){
			id = entries.size();
			entries.push_back(data_entry());
		} else {
			id = unused_entry_ids.top();
			unused_entry_ids.pop();
		}
		return id;
	}
	inline void delete_entry_id(size_t id){
		if(id != npos){
			entries[id].clear();
			unused_entry_ids.push(id);
		}
	}

	void fill_view(){
		while(view.row_data_unfilled()){
			view.fill_row_data(get_row_label(view.next_row_data()));
		}
		while(view.col_data_unfilled()){
			view.fill_col_data(get_col_label(view.next_col_data()));
		}
		while(view.unfilled()){
			view.fill(get_abbr_entry_str(view.next_row(),view.next_col()));
		}
	}

};

#endif
