/** 
 * 
 * 실행환경
 * windows11 / cmd
 * g++ (MinGW-W64 x86_64-ucrt-posix-seh) 14.2.0
 * UTF-8
 * 
 * 두 가지 과제에 대한 코드는 주석으로 구분되어있음
 * 각 과제에 대한 결과를 생성하여 csv 파일로 출력한다
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
#include <random>

using namespace std;


int ping(string target);    // target으로 1회 핑, RTT 반환

template <typename T>
void makeCSV(const vector<vector<T>>& totalData, const string name);       // 데이터를 받아서 csv 파일로 변환

int makeEvent();    // 5퍼센트, 10퍼센트 확률의 이벤트 발생


int main() {


    // problem 1; 핑 보내기 =========================================

    vector<vector<double>> totalData;   // RTT, E_RTT, D_RTT, Timeout

    double ERTT = 0;
    double DRTT = 0;

    // 100회 반복해여 totalData에 데이터 입력력
    for (int i = 0; i < 100; i++) {
        int RTT = ping("fmkorea.com");      // ping이 가능한 국내 사이트 중 사용자 수가 많은 사이트로 ping

        if (i != 0) {   // estimate RTT와 dev RTT 계산
            ERTT = (1 - 0.125) * ERTT + 0.125 * RTT;
            DRTT = (1 - 0.25) * DRTT + 0.25 * abs(RTT - ERTT);
        }

        double Timeout = ERTT + 4 * DRTT;       // Timeout 계산

        // data 생성하여 appand
        vector<double> data = {static_cast<double>(RTT), ERTT, DRTT, Timeout};
        totalData.push_back(data);
    }

    makeCSV(totalData, "ping");

    // ===========================================problem 1 end;





    // problem 2: TCP congestion control==========================

    vector<vector<int>> congData;    // cwnd, ssthresh, congestion 여부

    int cwnd = 1;
    int ssthresh = 32;

    for (int i = 0; i < 100; i++) {
        int state = makeEvent();        // 정상, timeout, 3 duplicate 여부 결정

        vector<int> data = {cwnd, ssthresh, state}; // 현 상태 기록 및 저장
        congData.push_back(data);

        switch (state) {    // state에 따른 cwnd, ssthresh 변화
        case 0:     // 정상 데이터 수신시
            if (cwnd < ssthresh) {
                cwnd *= 2;  // slow start, cumulative하게 Acks을 수신하였다고 가정, cwnd 각각 1씩 증가하여 총 2배 증가가
            }
            else {
                cwnd += 1;  // cumulative하게 Ack을 수신하여 1씩 선형적으로 증가
            }
            break;
        case 1:     // timeout 발생시
            ssthresh = cwnd / 2;
            cwnd = 1;
            break;
        case 2:     // 3 duplicate ack 발생시
            ssthresh = cwnd / 2;
            cwnd = cwnd / 2 + 3;
            break;
        }        

    }

    makeCSV(congData, "congestion");

    // ===========================================problem 2 end;



    return 0;
}

int makeEvent() {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> dis(0, 99);

    int result = dis(gen);

    if (result >= 50 && result <= 55) {     // 5%확률로 timeout
        return 1;
    }
    else if (result >= 60 && result <= 69) {    // 10% 확률로 3 duplicate ACKs
        return 2;
    }
    else {
        return 0;       // 정상 동작
    }

}

template <typename T>
void makeCSV(const vector<vector<T>>& totalData, const string name) {
    time_t timer = time(NULL);
    struct tm* t = localtime(&timer);

    string fileName = name + "_result_" + to_string(t->tm_mon + 1) + "-" + to_string(t->tm_mday) + "-" + to_string(t->tm_hour) + "-" + to_string(t->tm_min) + ".csv";

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
