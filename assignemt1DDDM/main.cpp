#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <sstream>
#include <math.h>
using namespace std;

vector<string> getAtt(string filename )
{
    vector<string> Att;
    ifstream infile;
    string data;
    infile.open(filename, ios::in);
    getline(infile, data);
    while (getline(infile, data))
    {
        string tmp;
        int pos=data.find(" ");
        data=data.substr(pos+1,data.size());
        stringstream ss(data);
        while (ss>>tmp) {
            Att.push_back(tmp);
        }
        
//        cout<<data<<endl;
    }
    infile.close();
    return Att;
}
vector<string> getQuery(string filename )
{
    vector<string> Query;
    ifstream infile;
    string data;
    infile.open(filename, ios::in);
    while (getline(infile, data))
    {
        int pos=data.find(" ");
        data=data.substr(pos+1,data.size());
        Query.push_back(data);
//        cout<<data<<endl;
    }
    infile.close();
    return Query;
}


vector<vector<int>> getAcc(string filename )
{
    vector<vector<int>> Acc;
    ifstream infile;
    string data;
    infile.open(filename, ios::in);
    getline(infile, data);
    while (getline(infile, data))
    {
        int pos=data.find(" ");
        data=data.substr(pos+1,data.size());
        int tmp;
//        cout<<data<<endl;
        vector<int> vec;
        stringstream ss(data);
        while(ss>>tmp)
        {
            vec.push_back(tmp);
//            cout<<tmp;
//            cout<<" ";
        }
//        cout<<endl;
        Acc.push_back(vec);
    }
    infile.close();
    return Acc;
}


int isFeatureUsed(string feature, string query)
{
    vector<string> vec;
    string tmp;
    int pos;
    while(query.find('(')!=string::npos)
    {
        pos=query.find('(');
        query.replace(pos, 1, " ");
    }
    while (query.find(')')!=string::npos)
    {
        pos=query.find(')');
        query.replace(pos, 1, " ");
    }
    while (query.find('=')!=string::npos)
    {
        pos=query.find('=');
        query.replace(pos, 1, " ");
    }
    while (query.find(',')!=string::npos)
    {
        pos=query.find(',');
        query.replace(pos, 1, " ");
    }
//    cout<<"Judge "+query<<endl;
    
    stringstream ss(query);

    while(ss>>tmp)
    {
        vec.push_back(tmp);
//        cout<<tmp;
//        cout<<endl;
    }
    
//    cout<<query<<endl;
    for (int i=0; i<vec.size(); i++) {
        if(feature==vec[i])
        {
            return 1;
        }
    }
    return 0;
}
int A_ik(int isUsed, vector<int> acc_k)
{
    int acc_sum=0;
    int a_ik=0;
    for(int i=0;i<acc_k.size();i++)
    {
        acc_sum=acc_sum+acc_k[i];
    }
    a_ik=isUsed*acc_sum;
    return a_ik;
}
int main(int argc, const char * argv[]) {
    // insert code here...
    string att_file="/Users/liuqinghao/Desktop/sample_case_2/att_2.txt";
    string query_file="/Users/liuqinghao/Desktop/sample_case_2/query_2.txt";
    string acc_file="/Users/liuqinghao/Desktop/sample_case_2/acc_2.txt";
    vector<string> att= getAtt(att_file);
    vector<string> queries= getQuery(query_file);
    vector<vector<int>> acc=getAcc(acc_file);
    int qyerySize=queries.size();
    int attSize=att.size();
    int usageMatrix[qyerySize][attSize];
    int AA[attSize][attSize];
//    cout<<"Usage Matrix"<<endl;
    for (int i=0; i<qyerySize; i++) {
        string query=queries[i];
//        cout<<query<<endl;
        for (int j=0; j<attSize; j++) {
            //if use or not
            string feature=att[j];
//            cout<<feature;
//            cout<<" ";
            usageMatrix[i][j]=isFeatureUsed(feature, query);
//            cout<<usageMatrix[i][j];
//            cout<<" ";
        }
//        cout<<endl;
    }
    
//    cout<<"ACC Matrix"<<endl;
//    for (int i=0; i<acc.size(); i++)
//    {
//        for (int j=0; j<acc[1].size(); j++) {
//            cout<<acc[i][j];
//            cout<<" ";
//        }
////    cout<<endl;
//    }

    for (int i=0; i<attSize; i++) {
        for (int j=0; j<attSize; j++)
        {
            long int sum_Aik_Ajk=0;
            long int sum_Aik=0;
            long int sum_Ajk=0;
            for (int k=0; k<qyerySize; k++) {
                sum_Aik_Ajk=sum_Aik_Ajk+A_ik(usageMatrix[k][i],acc[k])*A_ik(usageMatrix[k][j],acc[k]);
                sum_Aik=sum_Aik+A_ik(usageMatrix[k][i],acc[k]);
                sum_Ajk=sum_Ajk+A_ik(usageMatrix[k][j],acc[k]);
            }
            if (sum_Aik*sum_Ajk==0)
            {
                AA[i][j]=0;
            }
            else
            {
                AA[i][j]=ceil(sum_Aik_Ajk/sqrt(sum_Aik*sum_Ajk));
            }

            cout<<AA[i][j];
            cout<<" ";
        }
        cout<<endl;
    }
    
    
    return 0;
}



/*
 Instruction:
 Graders will compile and run your code with the following commands:
 
 g++ -std=c++11 AAGenerator.cpp -o runAA
 ./runAA att_1.txt query_1.txt acc_1.txt > aa_1.txt
 g++ -std=c++11 CAGenerator.cpp -o runCA
 ./runCA aa_1.txt > ca_1.txt
 
 att: label name--has the following features Ai
 
 Label Name
 A1 PNO
 A2 PNAME
 A3 BUDGET
 A4 LOC
 
 query: sql
 
 q1: SELECT BUDGET FROM PROJ WHERE PNO=Value
 q2: SELECT PNAME, BUDGET FROM PROJ
 q3: SELECT PNAME FROM PROJ WHERE LOC=Value
 q4: SELECT SUM(BUDGET) FROM PROJ WHERE LOC=Value
 
 
 
 acc: Access frequency matrix ACC, where Si denotes the i-th site
 
 S1 S2 S3
 q1 15 20 10
 q2 5 0 0
 q3 25 25 25
 q4 5 0 0
 
 Usage Matrix
 1 0 1 0
 0 1 1 0
 0 1 0 1
 0 0 1 1
 
 AA
 A1 A2 A3 A4
 A1 45 0 41 0
 A2 0 71 1 71
 A3 41 1 38 1
 A4 0 71 1 71
 */

/*
 AA Generator
 input:  all attributes of a relation, a set of queries
 and their access frequencies at different sites
 output: an affinity matrix AA
 
 
 COUNTRY
 COMPANY
 RANK
 LOC
 PRICE
 PID
 STREET
 PROPERTY
 TRANSCATION
 ADDR
 SIGNATURE
 PNO
 MANAGER
 PROVINCE
 DEAL
 EMAIL
 BLOCK
 PNAME
 DATA
 */

/*
 CA Generator
 input: an affinity matrix AA
 output:a clustered affinity matrix CA
 */
