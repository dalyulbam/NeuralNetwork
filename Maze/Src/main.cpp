#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <filesystem>
#include "NewErrorType.h"

/*
Class Type 

1. Map2D vector<vector<int>> 로 맵을 의미한다. 
2. Direction 은 vector<int> 로 움직일 수 있는 모든 가지수를 1번부터 나열한 것이다. 
3. History 는 stack<Data> 로 캐릭터가 움직였던 모든 지점의 데이터를 저장한 것이다. 
4. WayPoint 는 queue<Data> 로 게임에서 저장 지점을 의미하며, 처리해야 할 Thread 를 의미한다. 

*/

using namespace std;
using Map2D = vector<vector<char>>;
using Direction = vector<int>; 

constexpr int dir = 4; 
Direction dx { 1, -1, 0, 0 };
Direction dy { 0, 0, -1, 1 }; 

struct Data
{
	Data(int t, int x, int y) : time(t), x_cor(x), y_cor(y) {}
	int time, x_cor, y_cor;
};
using History = stack<Data>;
using WayPoint = queue<History>;

void copyStack(History& tc_tgt, History& tc_dest)
{
	// Initialize and make a temporary stack that is reversed. 
	History temp_stack_rev;
	while (!tc_dest.empty()) tc_dest.pop();

	// Move elements from original stack to auxiliary stack
	for (History tmp = tc_tgt; !tmp.empty(); tmp.pop()) {
		temp_stack_rev.push(tmp.top());
	}

	// Move elements back from auxiliary stack to copied stack
	while (!temp_stack_rev.empty()) {
		tc_dest.push(temp_stack_rev.top());
		temp_stack_rev.pop();
	}
}

/*
Char Type 

's' : start point 
'e' : end point 
'w' : wall 
'p' : path 


'a' : already stepped. Only available in visited_map  
*/

int check2DMap(Map2D map)
{
	int row = static_cast<int>(map.size()); 
	int col = static_cast<int>(map[0].size());
	int start_cnt = 0, end_cnt = 0; 
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (map[i][j] == 's') ++start_cnt;
			else if (map[i][j] == 'e') ++end_cnt; 
		}
	}
	if (start_cnt != 1 || end_cnt == 0)
	{
		throw ERROR_WRONG_MAP; 
		return false; 
	}
	return true; 
}

vector<string> split(const string& s, char delimiter) {
	vector<string> tokens;
	string token;
	istringstream tokenStream(s);

	while (getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

void strip(string& s, char delimiter, string::iterator init_start)
{
	string::iterator init = begin(s);  
	if (init_start != init) init = init_start; // time reducing 
	for (; init != end(s); ++init)
	{
		char curr_char = *init; 
		if (curr_char == delimiter)
		{
			string new_s_front(begin(s), init); 
			string new_s_back(init + 1, end(s)); 
			s = new_s_front + new_s_back; // edited 
			strip(s, delimiter, begin(s) + new_s_front.size()); 
			return; 
		}
	}
}

Map2D str2map(string raw_data)
{
	strip(raw_data, '\t', begin(raw_data)); 
	vector<string> rows = split(raw_data, '\n');
	Map2D my_map; 
	my_map.resize(rows.size());
	for (int i = 0; i < rows.size(); i++)
	{
		for (int j = 0; j < rows[i].size(); j++)
		{
			my_map[i].push_back((rows[i][j]));
		}
	}
	return my_map; 
}

int isValid(Map2D& map, Map2D& visited, Data& dt)
{
	int ret = SUCCESS_;
	int error_type = ERROR_;
	try
	{
		int right_border = static_cast<int>(map.size()); // +Y index is along the below side 
		int under_border = static_cast<int>(map[0].size()); // + X index is along the right side 

		// out of the range 
		error_type = ERROR_OUT_OF_RANGE;
		if (dt.x_cor < 0 || dt.x_cor >= right_border) return ERROR_;
		if (dt.y_cor < 0 || dt.y_cor >= under_border) return ERROR_;

		// The tile is unable to step on  
		error_type = ERROR_WRONG_TILE; 
		if (map[dt.x_cor][dt.y_cor] == 'w' || map[dt.x_cor][dt.y_cor] == '\0') return ERROR_;

		// The tile is already stepped before
		error_type = ERROR_ALREADY_STEPPED_TILE; 
		if (visited[dt.x_cor][dt.y_cor] == 'a') return ERROR_;
	}
	catch(int error_type_detail)
	{
		catchFunction(error_type, error_type_detail, "Something is ambiguous to tell whether is valid or invalid.");
	}
	return ret; 
}

int isSame(Data& dt1, Data& dt2)
{
	int ret = ERROR_; 
	int error_type = ERROR_DIFFERERNT_VALUE; 
	try
	{
		if (dt1.x_cor == dt2.x_cor && dt1.y_cor == dt2.y_cor) return SUCCESS_; 
		else return ret;
	}
	catch (int error_type_detail)
	{
		catchFunction(error_type, error_type_detail, "One of pair is hard to compare.");
	}
	return ret;
}

vector<Data> findChar(Map2D& map_data, char ch)
{
	vector<Data> dt_list; 
	for (int i = 0; i < map_data.size(); i++)
	{
		for (int j = 0; j < map_data[0].size(); j++)
		{
			if (map_data[i][j] == ch) dt_list.emplace_back(0, i, j);
		}
	}
	return dt_list;
}

Map2D makeDefaultMap(int row_size, int col_size)
{
	Map2D new_map;
	for (int i = 0; i < row_size; i++)
	{
		vector<char> temp_vec; 
		temp_vec.resize(col_size);
		new_map.emplace_back(temp_vec); 
	}
	return new_map; 
}

int initialize(Map2D& map_data, Map2D& visited)
{
	int ret = SUCCESS_;
	vector<Data> start_pnt_list = findChar(map_data, 's'); // 's' is unique here 
	if (start_pnt_list.size() > 1) return ERROR_;

	visited[start_pnt_list[0].x_cor][start_pnt_list[0].y_cor] = 'a'; // start_point foot print 
	return ret; 
}

int floodFillAlgorithm(WayPoint& way_point, History& record, Map2D& map, Map2D& visited, Data& target_pnt)
{
	int ret = SUCCESS_;
	int error_type = ERROR_; 
	try
	{
		if (way_point.empty())
		{
			History temp_hist;
			temp_hist.emplace(findChar(map, 's')[0]);  
			way_point.push(temp_hist);  
		}
		History hist = way_point.front(); // load the first save file... 
		way_point.pop();

		int turn = hist.top().time;
		int x_cor = hist.top().x_cor;
		int y_cor = hist.top().y_cor;
		int new_turn = turn + 1;

		// Seaking end point in EWSN direction 
		for (int i = 0; i < dir; i++)
		{
			int new_x_cor = x_cor + dx[i];
			int new_y_cor = y_cor + dy[i];
			

			Data new_data{ new_turn, new_x_cor, new_y_cor };
			if (!isValid(map, visited, new_data)) continue; // is valid step or not 
			visited[new_x_cor][new_y_cor] = 'a'; // step stamp 
			hist.push(new_data); // stack up 
			if (isSame(new_data, target_pnt)) // is end-point or not 
			{
				if (!record.empty() && record.top().time <= new_turn) continue;
				else copyStack(hist, record);
			}
			
			way_point.push(hist); // 1 direction 
			floodFillAlgorithm(way_point, record, map, visited, target_pnt); 
			hist.pop(); // revert and consider another history along different direction  
		}
	}
	catch(int error_type_detail)
	{
		catchFunction(error_type, error_type_detail, "Flood Fill Algorithm doesn't work");
	}
	return ret;
}

void saveResults(History& record, Map2D& map, string outer_directory)
{
	// Create a directory to save the CSV file if there is no such a directory. 
	filesystem::path outputDir(outer_directory); 
	if (!filesystem::exists(outer_directory)) 
	{
		error_code ec;
		if (!filesystem::create_directory(outputDir, ec)) {
			if (ec) {
				std::cerr << "Failed to create directory: " << ec.message() << std::endl;
				return;
			}
		}
	}

	ofstream csvFile_record(outer_directory + "topRecord.csv"), csvFile_map(outer_directory + "map.csv");
	if (!csvFile_record.is_open() || !csvFile_map.is_open())
	{
		std::cerr << "Failed to open the CSV file for writing" << std::endl;
		return;
	}

	// Vectorize Record
	vector<Data> save_Data; 
	while (!record.empty())
	{
		save_Data.emplace_back(record.top()); 
		record.pop();
	}

	// Write data to the CSV file
	for (int i = static_cast<int>(save_Data.size()) - 1; i >= 0; i--) 
	{
		const auto& row = save_Data[i];
		csvFile_record << row.time << "," << row.x_cor << "," << row.y_cor << "\n";
	}
	for (auto& row : map) 
	{
		for (int i = 0; i < static_cast<int>(row.size()); i++)
		{
			csvFile_map << row[i] << ",";
		}
		csvFile_map << "\n";
	}
	csvFile_record.close();
	csvFile_map.close(); 
	cout << "CSV file saved successfully!" << endl;
	return; 
}

int main()
{
	int ret = SUCCESS_; 
	int error_type = ERROR_; 
	string map_data ="\
		wwwwwww\n\
		sppwppe\n\
		wpwpwpw\n\
		wpppwpw\n\
		wwwpppw\n\
		wwwwwww\
	";
		
	try
	{
		History top_record_; // history is now record which will be gone, top_record is one of the fastest record 
		WayPoint way_point_;  // try_pnt is a queue of points which need to try flood-fill out. 
		Map2D my_map_ = str2map(map_data);
		Map2D visited_map_ = makeDefaultMap(static_cast<int>(my_map_.size()), static_cast<int>(my_map_[0].size()));
		check2DMap(my_map_);
		Data end_pnt_ = findChar(my_map_, 'e')[0];
		initialize(my_map_, visited_map_);
		floodFillAlgorithm(way_point_, top_record_, my_map_, visited_map_, end_pnt_); 
		saveResults(top_record_, my_map_, "logs/");
	}
	catch(int error_type_detail)
	{
		catchFunction(error_type, error_type_detail, "We have an issue.");
	}
}
