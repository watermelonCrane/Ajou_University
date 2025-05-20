/** 문제 1
 * tcp timeout을 계산하는 프로그램
 * 특정 사이트에 ping을 하여, 이로부터 RTT를 구함
 * 매번 ping을 사용하여 RTT 값이 구해질 때마다, TCP timeout 시간을 구함
 * 
 * 100회 실시 후, 이에 대한 실제 RTT값, 구해진 Estimated RTT와 TCP timeout값을 정리한다
 *  
 **/

#include <iostream>
#include <cstdio>
#include <string>
#include <memory>
#include <array>

using namespace std;

int pinging(string target, int count);

int main() {


    int run = pinging("8.8.8.8", 10);
    if (run) {
        cout << "error: run" << endl; 
    }
    

    return 0;
}


int pinging(string target, int count) {
    string command = "ping -n " + to_string(count) + " " + target;  // 명령어 생성. target: ping을 보낼 주소, count: 몇 번 보낼까요?

    unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
    if (!pipe) {    // 포인팅 실패시 에러 출력 후, return 1
        cerr << "ping fail" << endl;
        return 1;
    }

    array<char, 128> buffer;
    string result;

    FILE* file = fopen("result_1.txt", "w");    //결과를 저장할 파일 생성. 생성된 파일 인코딩 이슈 있음. utf-8또는 euc-kr로 열 것.
    if (file == nullptr) {  // 파일 오픈 실패시 2 리턴
        cerr << "file fail" << endl;
        return 2;
    }

    // pipe의 데이터를 result로 복사
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();    // result string에 결과 appand
        fputs(buffer.data(), file); // file에 결과 appand
    }

    fclose(file);       // file close

    //결과 출력
    cout << "Ping result:\n" << result << endl;

    return 0;   // 정상 출력시 0 리턴
}