#include<bits/stdc++.h>
using namespace std;
/*
    [x] See only how many trees are fell due to cur tree. Don't include the after-effects.
    [x] c*d*h should be higher
    [x] a tree might block a falling tree. In that case stop adding after-effects.
    mark filled only with distance = height
*/
/*
    [x] stays inside grid
    [x] no integer overflow

*/

/*
    Filled == 2 :=> tree was cut
    Filled == 1 :=> Tree can't be reached
*/

struct tree{
    // basic structure to contain all the information related to a tree
    int x,y;
    int height,thickness,unit_weight,unit_value;
    int affects_up,affects_down,affects_right,affects_left;

    tree(){
        affects_left = 0;
        affects_right = 0;
        affects_down = 0;
        affects_up = 0;
    }

    void initialise(int x_=0,int y_=0,int height_=0,int thickness_=0,int unit_weight_=0,int unit_value_=0){
        x = x_;
        y = y_;
        height = height_;
        thickness = thickness_;
        unit_weight = unit_weight_;
        unit_value = unit_value_;
    }
};

// to store the index of tree that is at given coordinates on the grid
int grid[10002][10002],size_of_grid;
int filled[10000+2][10000+2];

// an array that contains all information of a tree
tree Forest[10002];

pair<int,int> position = {0,0};

void calculate_Right();
void calculate_Left();
void calculate_Up();
void calculate_Down();
void mark_all(pair<int,int> pos, char dir);
void print_route(int x, int y, int xf, int yf);
int choose_opt(int ind1, int ind2, int time);

struct tree_comp{
    bool operator()(int index1, int index2){
        return Forest[index1].x + Forest[index1].y <= Forest[index2].x + Forest[index2].y;
    }
};

int main(){

    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);

    int time_allowed,number_of_trees;   
    cin >> time_allowed >> size_of_grid >> number_of_trees;

    for(int i=0;i<10002;i++){
        for(int j=0;j<10002;j++){
            filled[i][j] = -1;
            grid[i][j] = -1;
        }
    }

    int target_index = 1;

    for(int index=1; index <= number_of_trees; index++){

        int x,y,height,thickness,unit_weight,unit_value;
        cin >> x >> y >> height >> thickness >> unit_weight >> unit_value;

        grid[x][y] = index;
        
        Forest[index].initialise(x,y,height,thickness,unit_weight,unit_value);
    }

    calculate_Up();
    calculate_Left();
    calculate_Right();
    calculate_Down();

    while(time_allowed > 1){
        for(int i = 1; i <= number_of_trees;i++)
            target_index = choose_opt(target_index,i,time_allowed);
// assert(target_index <= number_of_trees && target_index > 0);

        auto temp = Forest[target_index];

        int x_needed = abs(position.first - temp.x) , y_needed = abs(position.second - temp.y);
        int time_required_for_this_tree = x_needed + y_needed + temp.thickness;

        if(filled[temp.x][temp.y] > 0)
            break;
        else if(time_required_for_this_tree >= time_allowed){
            filled[temp.x][temp.y] = 1;
            continue;
        }

        int max_data = max({temp.affects_up,temp.affects_down,temp.affects_right,temp.affects_left});

        if(max_data == temp.affects_up){
            print_route(position.first, position.second, temp.x, temp.y);
            cout << "cut up\n";
            mark_all(make_pair(temp.x,temp.y), 'u');
        }
        else if(max_data == temp.affects_down){
            print_route(position.first, position.second, temp.x, temp.y);
            cout << "cut down\n";
            mark_all(make_pair(temp.x,temp.y), 'd');
        }
        else if(max_data == temp.affects_left){
            print_route(position.first, position.second, temp.x, temp.y);
            cout << "cut left\n";
            mark_all(make_pair(temp.x,temp.y), 'l');
        }
        else if(max_data == temp.affects_right){
            print_route(position.first, position.second, temp.x, temp.y);
            cout << "cut right\n";
            mark_all(make_pair(temp.x,temp.y), 'r');
        }
        calculate_Up();
        calculate_Right();
        calculate_Left();
        calculate_Down();

        position = make_pair(temp.x, temp.y);
        time_allowed -= time_required_for_this_tree;
    }

    return 0;
}

int choose_opt(int ind1, int ind2, int time_remaining){
    int xi = Forest[ind2].x, yi = Forest[ind2].y, ti = Forest[ind2].thickness;
    int xii = Forest[ind1].x, yii = Forest[ind1].y, tii = Forest[ind1].thickness;
    int m1 = max({Forest[ind1].affects_up,Forest[ind1].affects_down,Forest[ind1].affects_right,Forest[ind1].affects_left});
    int m2 = max({Forest[ind2].affects_up,Forest[ind2].affects_down,Forest[ind2].affects_right,Forest[ind2].affects_left});
    int x_dis1 = abs(xii - position.first), x_dis2 = abs(xi - position.first), y_dis1 = abs(yii - position.second), y_dis2 = abs(yi - position.second);
    int t_r = time_remaining;
		int xi_modified=(xi - position.first)*(xi - position.first);
	int yi_modified=(yi - position.second)*(yi- position.second);
	int xii_modified=(xii - position.first)*(xii - position.first);
	int yii_modified=(yii - position.second)*(yii- position.second);
    if(ind1 <= 0)
        return ind2;
    else if(filled[xi][yi] > 0)
        return ind1;
    else if(filled[xii][yii] > 0)
        return ind2;

    // profit per unit dist from cur pos    

    else if ((x_dis2 + y_dis2 + ti) <= t_r && (float)(m2)/(x_dis2 + y_dis2 + ti) > (float)(m1)/(x_dis1 + y_dis1 + tii))
		if (((x_dis2 + y_dis2 + ti) <= (x_dis1 + y_dis1 + tii)) && (m2>=m1))       
		 	return ind2;
		else if((xii_modified + yii_modified)>=(xi_modified + yi_modified))
			return ind2;
		else return ind2;
		//else if( (m1 > m2 * ((x_dis2 + y_dis2 + ti)/(x_dis1 + y_dis1 + tii))*((x_dis2 + y_dis2 + ti)/(x_dis1 + y_dis1 + tii))) && ((x_dis2 + y_dis2 + ti) >= (x_dis1 + y_dis1 + tii)))
			//return ind1;

    //else if ((x_dis1 + y_dis1 + tii) <= t_r && (float)(m2)/(x_dis2 + y_dis2 + ti) < (float)(m1)/(x_dis1 + y_dis1 + tii))
      //  return ind1;
		//else if(((t_r-(abs(x_dis1+y_dis1)+tii))>0) && (float)m1/m2 > (float)abs(t_r-(abs(x_dis2+y_dis2)+ti))/abs(t_r-(abs(x_dis1+y_dis1)+tii))){
			//if((t_r-(abs(x_dis2+y_dis2)+ti))>=0)
            	//return ind2;
			//else if((t_r-(abs(x_dis1+y_dis1)+tii))>=0)
				//return ind1;}
/*     else if ((float)(m1)/(t_r - abs(xi+yi-sum) + ti) > (float)(m2)/(t_r - abs(xii+yii-sum) + tii) && (abs(xii+yii-sum)+tii)<=t_r)
         return ind1;
     else if((float)m1/m2 > (float)abs(t_r-abs(xi+yi-sum)+ti)/abs(t_r-abs(xii+yii-sum)+tii)){
             return ind1;
    // }

    else if(x_dis2 + y_dis2 + ti < x_dis1 + y_dis1 + tii){
        if((float)m1/(x_dis1 + y_dis1 + tii) > (float)m2/(x_dis2 + y_dis2 + ti)){
            return ind1;
        }
        else return ind2;
    }
    // else if((float)m2/m1 > (float)(t_r-x_dis1 + y_dis1 + tii)/(t_r-x_dis2 + y_dis2 + ti)){
    //         return ind2;
    // }


    else if(x_dis2 + y_dis2 + tii == x_dis1 + y_dis1 + tii){
        if(m1 > m2)
            return ind1;
        else return ind2;
    }
    // else if(x_dis2 + y_dis2 + tii > x_dis1 + y_dis1 + tii){
    //     if((float)m2/(x_dis2 + y_dis2 + ti) > (float)m1/(x_dis1 + y_dis1 + tii))
    //         return ind2;
    //     else return ind1;
    // }
		else if((xii_modified + yii_modified)<=(xi_modified + yi_modified))
		return ind1;*/
// un-commenting the following increases profit, but brings on WA on test case 12
    // else if(m1 > m2) {
    //     return ind1;
    // }
    // else if(m2 > m1) {
    //     return ind2;
    // }
    return ind1;
    // return which is closer to origin
}

void print_route(int x, int y, int xf, int yf){
    int x_diff = (x < xf ? 1:-1);
    int y_diff = (y < yf ? 1:-1);

    vector<string> x_dir = {"move left\n","move right\n"};
    vector<string> y_dir = {"move down\n","move up\n"};

    while(x != xf){
        cout << x_dir[x_diff == 1];
        x += x_diff;
    }

    while(y != yf){
        cout << y_dir[y_diff == 1];
        y += y_diff;
    }
    // stays inside grid
    // assert(x >= 0 && x <size_of_grid);
    // assert(y >= 0 && y <size_of_grid);
    // assert(yf >= 0 && yf <size_of_grid);
    // assert(xf >= 0 && xf <size_of_grid);
}

// pos is position where we need to go and cut the tree
void mark_all(pair<int,int> pos, char dir){
    int x = pos.first, y = pos.second, tar_ind, hei = Forest[grid[x][y]].height, cur_x = x, cur_y = y;
    filled[x][y] = 2;
    switch (dir){
        case 'l':
            tar_ind = max(0,pos.first - hei + 1);
            while(x >= tar_ind){
                if(grid[x][y] != -1 && filled[x][y] != 2) {
                    int c = Forest[grid[x][y]].unit_weight, h = Forest[grid[x][y]].height, t = Forest[grid[x][y]].thickness;
                    int c_ = Forest[grid[cur_x][cur_y]].unit_weight, h_ = Forest[grid[cur_x][cur_y]].height, t_ = Forest[grid[cur_x][cur_y]].thickness;
                        filled[x][y] = 2;
                    if (c*h*t < c_*h_*t_) {
                        // tar_ind = min(tar_ind,max(0,x - Forest[grid[x][y]].height));
                        // if (x == tar_ind && tar_ind != 0) {
                        //     break;
                        // }
                        tar_ind = max(0,x - Forest[grid[x][y]].height + 1);
                        cur_x = x;
                    }
                    // else break;
                }
                x--;
            }
            break;
        case 'r':
            tar_ind = min(pos.first+hei-1,size_of_grid-1);
            while(x <= tar_ind){
                if(grid[x][y] != -1 && filled[x][y] != 2) {
                    int c = Forest[grid[x][y]].unit_weight, h = Forest[grid[x][y]].height, t = Forest[grid[x][y]].thickness;
                    int c_ = Forest[grid[cur_x][cur_y]].unit_weight, h_ = Forest[grid[cur_x][cur_y]].height, t_ = Forest[grid[cur_x][cur_y]].thickness;
                        filled[x][y] = 2;
                    if (c*h*t < c_*h_*t_) {
                        // if (x == tar_ind && tar_ind != size_of_grid-1) {
                        //     break;
                        // }
                        // tar_ind = max(tar_ind,min(size_of_grid-1,x + Forest[grid[x][y]].height));
                        tar_ind = min(size_of_grid-1,x + Forest[grid[x][y]].height-1);
                        cur_x = x;
                    }
                    else break;
                }
                x++;
            }
            break;
        case 'u':
            tar_ind = min(pos.second+hei-1,size_of_grid-1);
            while(y <= tar_ind){
                if(grid[x][y] != -1 && filled[x][y] != 2) {
                    int c = Forest[grid[x][y]].unit_weight, h = Forest[grid[x][y]].height, t = Forest[grid[x][y]].thickness;
                    int c_ = Forest[grid[cur_x][cur_y]].unit_weight, h_ = Forest[grid[cur_x][cur_y]].height, t_ = Forest[grid[cur_x][cur_y]].thickness;
                        filled[x][y] = 2;
                    if (c*h*t < c_*h_*t_) {
                        // if (y == tar_ind && tar_ind != size_of_grid-1) {
                        //     break;
                        // }
                        // tar_ind = max(tar_ind,min(size_of_grid-1,y + Forest[grid[x][y]].height));
                        tar_ind = min(size_of_grid-1,y + Forest[grid[x][y]].height-1);
                        cur_y = y;
                    }
                    else break;
                }
                y++;
            }
            break;
        case 'd':
            tar_ind = max(0,pos.second - hei + 1);
            while(y >= tar_ind){
                if(grid[x][y] != -1 && filled[x][y] != 2) {
                    int c = Forest[grid[x][y]].unit_weight, h = Forest[grid[x][y]].height, t = Forest[grid[x][y]].thickness;
                    int c_ = Forest[grid[cur_x][cur_y]].unit_weight, h_ = Forest[grid[cur_x][cur_y]].height, t_ = Forest[grid[cur_x][cur_y]].thickness;
                        filled[x][y] = 2;
                    if (c*h*t < c_*h_*t_) {
                        // if (y == tar_ind && tar_ind != 0) {
                        //     break;
                        // }
                        // tar_ind = min(tar_ind,max(0,y - Forest[grid[x][y]].height));
                        tar_ind = max(0,y - Forest[grid[x][y]].height + 1);
                        cur_y = y;
                    }
                    else break;
                }
                y--;
            }
            break;
    }
}


void calculate_Down(){
    for(int y_index = 0;y_index < size_of_grid; y_index++){
        for(int x_index = 0; x_index <= size_of_grid-1;x_index++){
            // if there is no tree at the given co-ordinates
            if(grid[x_index][y_index] == -1)
                continue;
            if(filled[x_index][y_index] == 2)
                continue;

            int p_ = Forest[grid[x_index][y_index]].unit_value, c_ = Forest[grid[x_index][y_index]].unit_weight, d_ = Forest[grid[x_index][y_index]].thickness, h_ = Forest[grid[x_index][y_index]].height;
            Forest[grid[x_index][y_index]].affects_down = p_*h_*d_;
            int x = x_index, y = y_index;           
            
            int down_affected_index = y_index - Forest[grid[x_index][y_index]].height;
            // handle case where down_affected_index < 0
            down_affected_index = max(down_affected_index,0);
            int cur_iteration = y_index - 1;

            // while(down_affected_index < y_index){
            while(cur_iteration > down_affected_index){
                if(grid[x_index][cur_iteration] != -1 && filled[x_index][cur_iteration] != 2){

                    tree variable = Forest[grid[x_index][cur_iteration]];
                    int p = variable.unit_value, c = variable.unit_weight, d = variable.thickness, h = variable.height;
                    int _p = Forest[grid[x][y]].unit_value, _c = Forest[grid[x][y]].unit_weight, _d = Forest[grid[x][y]].thickness, _h = Forest[grid[x][y]].height;

                    if(_c*_d*_h < c*d*h)
                        break;

                    Forest[grid[x_index][y_index]].affects_down += p*h*d;
        // assert(Forest[grid[x_index][y_index]].affects_down >= 0);
                    y = cur_iteration;
                    down_affected_index = max(y - h,0);
                    // break;
                }

                cur_iteration-- ;
            }
        }
    }
}

void calculate_Up(){
    for(int y_index = size_of_grid-1;y_index >= 0; y_index--){
        for(int x_index = 0; x_index <= size_of_grid-1;x_index++){
            if(grid[x_index][y_index] == -1)
                continue;
            if(filled[x_index][y_index] == 2)
                continue;

            int p_ = Forest[grid[x_index][y_index]].unit_value, c_ = Forest[grid[x_index][y_index]].unit_weight, d_ = Forest[grid[x_index][y_index]].thickness, h_ = Forest[grid[x_index][y_index]].height;
            Forest[grid[x_index][y_index]].affects_up = p_*h_*d_;
            int x = x_index, y = y_index;

            int up_affected_index = y_index + Forest[grid[x_index][y_index]].height;
            // handle case where up_affected_index < 0
            up_affected_index = min(up_affected_index,size_of_grid-1);
            int cur_iteration = y_index + 1;

            // while(up_affected_index > y_index){
            while(cur_iteration < up_affected_index){
                if(grid[x_index][cur_iteration] != -1 && filled[x_index][cur_iteration] != 2){

                    tree variable = Forest[grid[x_index][cur_iteration]];
                    int p = variable.unit_value, c = variable.unit_weight, d = variable.thickness, h = variable.height;
                    int _p = Forest[grid[x][y]].unit_value, _c = Forest[grid[x][y]].unit_weight, _d = Forest[grid[x][y]].thickness, _h = Forest[grid[x][y]].height;

                    if(_c*_d*_h < c*d*h)
                        break;

                    Forest[grid[x_index][y_index]].affects_up += p*h*d;
        // assert(Forest[grid[x_index][y_index]].affects_up >= 0);
                    y = cur_iteration;
                    up_affected_index = min(y + h,size_of_grid-1);
                    // break;
                }

                cur_iteration++ ;
            }
        }
    }
}

void calculate_Left(){
    for(int y_index = 0;y_index < size_of_grid; y_index++){
        for(int x_index = 0; x_index <= size_of_grid-1;x_index++){
            if(grid[x_index][y_index] == -1)
                continue;
            if(filled[x_index][y_index] == 2)
                continue;

            int p_ = Forest[grid[x_index][y_index]].unit_value, c_ = Forest[grid[x_index][y_index]].unit_weight, d_ = Forest[grid[x_index][y_index]].thickness, h_ = Forest[grid[x_index][y_index]].height;
            Forest[grid[x_index][y_index]].affects_left = p_*h_*d_;
            int x = x_index, y = y_index;

            int left_affected_index = x_index - Forest[grid[x_index][y_index]].height;
            // handle case where left_affected_index < 0
            left_affected_index = max(left_affected_index,0);
            int cur_iteration = x_index - 1;

            // while(left_affected_index < x_index){
            while(cur_iteration > left_affected_index){
                if(grid[cur_iteration][y_index] != -1 && filled[cur_iteration][y_index] != 2){

                    tree variable = Forest[grid[cur_iteration][y_index]];
                    int p = variable.unit_value, c = variable.unit_weight, d = variable.thickness, h = variable.height;
                    int _p = Forest[grid[x][y]].unit_value, _c = Forest[grid[x][y]].unit_weight, _d = Forest[grid[x][y]].thickness, _h = Forest[grid[x][y]].height;

                    if(_c*_d*_h < c*d*h)
                        break;

                    Forest[grid[x_index][y_index]].affects_left += p*h*d;
        // assert(Forest[grid[x_index][y_index]].affects_left >= 0);
                    x = cur_iteration;
                    left_affected_index = max(x - h,0);
                }

                cur_iteration-- ;
            }
        }
    }
}

void calculate_Right(){
    for(int y_index = 0;y_index < size_of_grid; y_index++){
        for(int x_index = size_of_grid-1; x_index >= 0;x_index--){
            if(grid[x_index][y_index] == -1)
                continue;
            if(filled[x_index][y_index] == 2)
                continue;

            int p_ = Forest[grid[x_index][y_index]].unit_value, c_ = Forest[grid[x_index][y_index]].unit_weight, d_ = Forest[grid[x_index][y_index]].thickness, h_ = Forest[grid[x_index][y_index]].height;
            Forest[grid[x_index][y_index]].affects_right = p_*h_*d_;
            int x = x_index, y = y_index;

            int right_affected_index = x_index + Forest[grid[x_index][y_index]].height;
            right_affected_index = min(right_affected_index,size_of_grid - 1);

            int cur_iteration = x_index + 1;

            // while(right_affected_index > x_index){
            while(cur_iteration < right_affected_index){
                if(grid[cur_iteration][y_index] != -1 && filled[cur_iteration][y_index] != 2){

                    tree variable = Forest[grid[cur_iteration][y_index]];
                    int p = variable.unit_value, c = variable.unit_weight, d = variable.thickness, h = variable.height;
                    int _p = Forest[grid[x][y]].unit_value, _c = Forest[grid[x][y]].unit_weight, _d = Forest[grid[x][y]].thickness, _h = Forest[grid[x][y]].height;

                    if(_c*_d*_h < c*d*h)
                        break;

                    Forest[grid[x_index][y_index]].affects_right += p*d*h;
        // assert(Forest[grid[x_index][y_index]].affects_right >= 0);
                    x = cur_iteration;
                    right_affected_index = min(x + h,size_of_grid-1);
                    // break;
                }

                cur_iteration++ ;
            }
        }
    }
}
