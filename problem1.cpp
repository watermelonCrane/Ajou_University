/** 문제 1
 * tcp timeout을 계산하는 프로그램
 * 특정 사이트에 ping을 하여, 이로부터 RTT를 구함
 * 매번 ping을 사용하여 RTT 값이 구해질 때마다, TCP timeout 시간을 구함
 * 
 * 100회 실시 후, 이에 대한 실제 RTT값, 구해진 Estimated RTT와 TCP timeout값을 정리한다
 * 
 * 실행환경
 * OS: windows11
 * shell: cmd
 * 인코딩 EUC-KR or UTF-8
 * 
 *  
 **/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <ctime>

using namespace std;


int ping(string target);    // target으로 1회 핑, RTT 반환
void makeCSV(const vector<vector<double>>& totalData);       // 데이터를 받아서 csv 파일로 변환환

int main() {

    vector<vector<double>> totalData;   // RTT, E_RTT, D_RTT, Timeout

<<<<<<< HEAD
    int run = pinging("8.8.8.8", 10);
    if (run) {
        cout << "error: run" << endl; 
    }
=======
    double ERTT = 0;
    double DRTT = 0;


    // 100회 반복해여 totalData에 데이터 입력력
    for (int i = 0; i < 100; i++) {
        int RTT = ping("fmkorea.com");
>>>>>>> dev
    
        if (i != 0) {
            ERTT = (1 - 0.125) * ERTT + 0.125 * RTT;
            DRTT = (1 - 0.25) * DRTT + 0.25 * abs(RTT - ERTT);
        }

        double Timeout = ERTT + 4 * DRTT;

        // data 생성하여 appand
        vector<double> data = {static_cast<double>(RTT), ERTT, DRTT, Timeout};
        totalData.push_back(data);
    }

    makeCSV(totalData);

    return 0;
}

void makeCSV(const vector<vector<double>>& totalData) {
    time_t timer = time(NULL);
    struct tm* t = localtime(&timer);

    string fileName = "result_" + to_string(t->tm_mon + 1) + "-" + to_string(t->tm_mday) + "-" + to_string(t->tm_hour) + "-" + to_string(t->tm_min) + ".csv";

    ofstream file(fileName);
    if (!file.is_open()) {
        cerr << "file error" << endl;
        exit(1);
    }

    for (auto& x: totalData) {
        for (auto iter = x.begin(); iter != x.end(); iter++) {
            file << *iter;
            if (next(iter) != x.end()) file << ",";
        }
        
        file << endl;
    }

    file.close();
}

int ping(string target) {
    string command = "chcp 437 & ping -n 1 " + target; // ping 명령어 생성

    array<char, 128> buffer;    // 문자열 변환에 사용할 버퍼퍼
    string result;      // 결과를 저장할 문자열열

    unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
    if (!pipe) {    // 포인팅 실패세 에러 출력 및 -1 반환환
        cerr << "ping fail" << endl;
        exit(1);
    }

    // result에 ping 결과 저장
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    // result에서 RTT Get
    size_t start = result.find("time=") + 5;
    size_t end = result.find("ms", start);

    return stoi(result.substr(start, end - start)); // RTT return

}
