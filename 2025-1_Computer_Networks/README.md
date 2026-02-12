# TCP Test Programming

- _**See report for a detailed description of the program result**_

### Goal of Program
1. Calcuate TCP timeout via ping
2. TCP Congestion Control Simulation

<details>
<summary><b>Details <i>(Korean)</i></b></summary>

<h3>1. tcp timeout 계산하는 프로그램을 작성하시오.</h3>
<ul>
<li>특정 사이트에 ping 을 하여, 이로 부터 RTT를 구함</li>
<li>매번 ping을 사용하여 RTT 값이 구해질때마다, TCP timeout 시간을 구함</li>
<li>100회 실시하여, 이에 대한 실제 RTT 값과, 구해진 Estimated RTT와 TCP timeout 값을 그래프로 그리시오.</li>
<li>동일한 사이트에 대하여, 오전, 오후, 저녁의 시간대에 실시하여, 어떤 변화가 있는지를 관찰하시오.</li>
<li>
    제출내용은 다음과 같이 하시오.
    <ul>
        <li>작성 프로그램 (프로그램 설명 내용을 주석으로 포함)</li>
        <li>시간대에 따른 결과 그래프 (사이트, 오전시간, 오후시간, 저녁시간)</li>
        <li>결과에 대한 분석 내용</li>
    </ul>
</li>
</ul>


<h3>2. Slow start, Congestion Avoidance, Fast Recovery을 결합한 TCP congestion control 프로그램을 작성하시오.</h3>
<ul>
<li>초기 cwnd = 1</li>
<li>초기 ssthresh = 32</li>
<li>
    매 라운드마다 다음과 같은 이벤트 발생
    <ul>
        <li>5%의 확률로 timeout 발생</li>
        <li>10%의 확률로 3 duplicate ACKs 발생</li>
    </ul>
</li>
<li>작성한 프로그램으로 100라운드를 실시하여, cwnd와 ssthresh의 변화 그래프를 작성</li>
<li>위의 실행을 3회 실시</li>
<li>
    제출내용은 다음과 같이 하시오.
    <ul>
        <li>작성 프로그램 (프로그램 설명 내용을 주석으로 포함)</li>
        <li>각 실행에 대한 결과 그래프 (그래프에, timeout과 3 dup. ACK 이벤트 발생시간 표시)</li>
        <li>결과에 대한 분석 내용 (각 실행에서 timeout과 3 dup. ACK이 몇번 발생하였고, 각 발생한 후에 동작에 대한 설명)</li>
    </ul>
</li>
</ul>

<h4>이에 대한 결과 리포트는 report.pdf를 참고해주세요.</h4>

</details>

## Description of program
### Functions

```C
// Ping target Once. Return RTT after parsing.
int ping(string target);

// Create events with 5 percent and 10 percent probability.
// This related in problem 2.
int makeEvent();

// Save the received vector data as a CSV file.
void makeCSV(...);

```

### Problem 1
- By repeating the ping(```int ping(...)```) 100 times, Estimate RTT and Dev RTT are obtained through the RTT. Through this, the time-out time is calculated.
- Extract this data to CSV. See ```report.pdf``` for results on this.

### Problem 2
- Simulate TCP Congestion Control.
- Determine whether or not **normal**, **timeout**, and **3-duplicate-ACK** occur with 5 percent and 10 percent probabilities, and adjust the ```cwnd``` and ```ssthresh``` values.
- Repeat this 100 times and extract it with CSV. See ```report.pdf``` for the results.


## Result Preview
<p align="center">
    <img src="./result_images/Problem1.png">
    <br><br><br>
    <img src="./result_images/Problem2.png">
</p>

> Please Check the "report.pdf" for the detail.

