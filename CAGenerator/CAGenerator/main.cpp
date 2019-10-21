#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;

vector<vector<int>> getAA(string filename )
{
    vector<vector<int>> AA;
    ifstream infile;
    string data;
    infile.open(filename, ios::in);
    while (getline(infile, data))
    {
        int tmp;
        vector<int> vec;
        stringstream ss(data);
        while(ss>>tmp)
        {
            vec.push_back(tmp);
//                        cout<<tmp;
//                        cout<<" ";
        }
//                cout<<endl;
        AA.push_back(vec);
    }
    infile.close();
    return AA;
}
//index strat from 1
int bond(vector<vector<int>> AA, int left, int right)
{
    left=left-1;
    right=right-1;
    int bondvalue=0;
    if(left<0||right>=AA.size()||right<0||left>=AA.size())
    {
        return bondvalue;
    }
    for (int i=0; i<AA.size(); i++) {
        bondvalue=bondvalue+AA[i][left]*AA[i][right];
    }
    return bondvalue;
}

int cont(vector<vector<int>> AA, int left,int middle, int right)
{
    if(left <= 0) {
        return 2*bond(AA,middle, right);
    }

    // if rightmost case
    if(right >= middle + 1) {
        return 2*bond(AA,left, middle);
    }
    return 2*bond(AA, left, middle)+2*bond(AA, middle, right)-2*bond(AA, left, right);
}
vector<int> getColumns(vector<vector<int>> AA, int column_index)
{
    vector<int> column;
    for (int i=0; i<AA[0].size(); i++) {
        column.push_back(AA[i][column_index-1]);
    }
    return column;
}

void placeColumn(vector<vector<int>> &CA,vector<int> column,int index)
{
    for (int i=0; i<CA.size(); i++) {
        CA[i][index-1]=column[i];
    }
}


vector<int> getRow(vector<vector<int>> AA, int row_index)
{
    vector<int> row;
    for (int i=0; i<AA[0].size(); i++) {
        row.push_back(AA[row_index-1][i]);
    }
    return row;
}

void placeRow(vector<vector<int>> &CA,vector<int> row,int index)
{
    for (int i=0; i<CA.size(); i++) {
        CA[index-1][i]=row[i];
    }
}

void showRecord(vector<int> record)
{
        for (int j=0; j<record.size(); j++) {
            cout<<"\tA";
            cout<<record[j];
        }
        cout<<endl;
}


void showVector(vector<vector<int>> AA,vector<int> record)
{
    showRecord(record);
    for (int i=0; i<AA.size(); i++) {
        cout<<"A";
        cout<<record[i];
        cout<<"\t";

        for (int j=0; j<AA.size(); j++) {
            cout<<AA[i][j];
            cout<<" ";
        }
        cout<<endl;
    }
}

//all index start from 1;
int main(int argc, const char * argv[]) {
    vector<vector<int>> AA;
    string aa_file="/Users/liuqinghao/Desktop/sample_case_2/aa_2.txt";
    AA=getAA(aa_file);
    int length=AA.size();
    vector<vector<int> > CA(length, vector<int>(length,0));
    vector<int> record(length);
    placeColumn(CA, getColumns(AA, 1), 1);
    placeColumn(CA, getColumns(AA, 2), 2);
    record[0]=1;
    record[1]=2;
//    cout<<bond(CA, 3, 1)<<endl;
//    cout<<cont(CA, 0, 3, 1)<<endl;
//
    int index=3;
    while (index<=CA.size()) {
        placeColumn(CA, getColumns(AA, index), index);

        int contirbution=0;
        int maxCont=0;
        int place=0;
        for (int i=1; i<index; i++) {
            contirbution=cont(CA, i-1, index, i);
//            cout<<contirbution<<endl;
            if (contirbution>maxCont)
            {
                place=i;
                maxCont=contirbution;
            }
        }

        contirbution=cont(CA, index-1, index, index+1);
//        cout<<contirbution<<endl;

        if (contirbution>maxCont)
        {
            place=index;
            maxCont=contirbution;
        }

//        cout<<place<<endl;
//        cout<<endl;
        
        if(place<1)
        {
            for (int j=index; j>1; j--) {
                placeColumn(CA, getColumns(CA, j-1), j);
                //start from 0
                record[j-1]=record[j-2];
            }
            placeColumn(CA, getColumns(AA, index), 1);
            record[0]=index;

        }
        else{
            for (int j=index; j>=place; j--) {
                placeColumn(CA, getColumns(CA, j-1), j);
                record[j-1]=record[j-2];
            }
            placeColumn(CA, getColumns(AA, index), place);
            record[place-1]=index;
        }
        
        
//        break;
        index++;
    }
    
    
    vector<vector<int> > finalCA(length, vector<int>(length,0));
    
    for (int i=1; i<=record.size(); i++) {
        placeRow(finalCA, getRow(CA,record[i-1]), i);
    }

    
    showVector(finalCA,record);
//    showRecord(record);

    return 0;
}
