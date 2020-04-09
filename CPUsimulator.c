#include<stdio.h>
#include<stdlib.h>
#include<time.h> //random를 위한
#include<math.h>//abs를 위한
#include<conio.h> 

typedef struct process{//프로세스정보
	int pid;
	int CPU_burst_time;
	int IO_burst_time;
	int arrival_time;
	int priority;
	int waiting;
	int turnaround;
	int start[20];
	int end[20];
}Process;

Process create_process(int n);//프로세스 생성
void FCFS(int n, Process *p);//First Come First Server Algorism
void SJF(int n, Process *p);//Shortest Job First Algorism 
void preemptive_SJF(int n, Process *p);//Preemptive 가능한 SJF 알고리즘
void Priority(int n, Process *p);//우선순위 알고리즘
void preemptive_Priority(int n, Process *p);//preemptive 가능한 우선순위 알고리즘
void RoundRobin(int n, Process *p);
void evalutaion(int n, Process *p);//avg turnaround, avg waiting 구하기
float gantt(int n, Process *p);//gantt chart 그리기

int main(){
	int i=0;
	int n=0;
	int key=0;
	Process p[20];	

	srand((unsigned int)time(NULL) ); 
	printf("How many process?(max20) ");
	scanf_s("%d",&n);
	
	printf("starting process state\n");
	for(i=0; i<n; i++){
		p[i] = create_process(i);
		printf("pid: %d  CPU_burst_time: %d  IO_burst_time: %d  arrival_time: %d  priority: %d\n",p[i].pid,p[i].CPU_burst_time,p[i].IO_burst_time,p[i].arrival_time, p[i].priority);
	}

	FCFS(n, p);
	SJF(n,p);
	preemptive_SJF(n,p);
	Priority(n,p);
	preemptive_Priority(n,p);
	RoundRobin(n,p);

	key = _getch();
	while(key!= 97){
		key =_getch();
	}
}

Process create_process(int n){
	int i;
	Process p;	
	p.pid = n;
	p.CPU_burst_time = rand()%10+1;
	p.IO_burst_time = rand()%1;
	if(n==0) p.arrival_time=0;
	else p.arrival_time = rand()%15;
	//p.arrival_time = rand()%10;
	p.priority = rand()%10;
	p.waiting =0;
	p.turnaround =0;
	for(i=0; i<20; i++){
		p.start[i]= 0;
		p.end[i] =0;
	}
	return p;
}

void FCFS(int n, Process *p){
	int i=0, j=0;
	int waiting_time =0;
	Process fcfs[20];
	Process tmp;

	for(i=0; i<n; i++){
		fcfs[i]=p[i];
	}
	printf("\n====================================================================================================================\n");
	printf("First Come First Served Algorism\n");

	for(i=0; i<n-1; i++){//선택정렬 같이도착시 pid가 작은수가 우선
		for(j=i+1;j<n; j++){
			if(fcfs[i].arrival_time > fcfs[j].arrival_time){
				tmp = fcfs[i];
				fcfs[i] = fcfs[j];
				fcfs[j] = tmp;
			}
		}
	}
/*	for(i=0; i<n; i++){
		printf("pid: %d  CPU_burst_time: %d  arrival_time: %d  priority: %d\n",fcfs[i].pid,fcfs[i].CPU_burst_time,fcfs[i].arrival_time, fcfs[i].priority);
	}
*/
	for(i=0; i<n; i++){
		while(waiting_time < fcfs[i].arrival_time)
			waiting_time++;
		
		fcfs[i].start[0] = waiting_time;	
		fcfs[i].waiting = waiting_time - fcfs[i].arrival_time;

		printf("ready queue     : ");
		
		for(j=i+1; j<n; j++){
			if(fcfs[j].arrival_time>waiting_time)
				break;
			printf("p%d  ",fcfs[j].pid);
		}
		printf("\nrunning process : p%d\n\n", fcfs[i].pid);

		waiting_time += fcfs[i].CPU_burst_time;
		fcfs[i].end[0] = waiting_time;
		fcfs[i].turnaround = fcfs[i].waiting + fcfs[i].CPU_burst_time; 
	}
	evalutaion(n,fcfs);
}

void SJF(int n, Process *p){
	int i=0, j=0;
	int num=n;
	int start=0, ready_count=0, terminate_count=0;
	Process running;
	Process sjf[20];
	Process ready_queue[20];
	Process terminated[20];
	Process tmp;

	for(i=0; i<n; i++){
		sjf[i]=p[i];
	}
	running = sjf[0];
	printf("\n====================================================================================================================\n");
	printf("Shortest Job First Algorism\n");
	while(terminate_count != n){//모든 프로세스가 종료될 때까지 진행
		if(num!=0){ //도착한 것 ready_queue에 넣기
			for(i=0; i<num; i++){
				if(sjf[i].arrival_time <= start){
					ready_queue[ready_count] = sjf[i];
					for(j=i;j<num;j++){
						sjf[j] =sjf[j+1];
					}
					i--;
					num--;
					ready_count++;
				}
			}
		}

		if(ready_count>0){//ready_queue에 하나 이상있을 시 running한다.
			for(i=0; i<ready_count-1; i++){
				for(j=i+1;j<ready_count; j++){
					if(ready_queue[i].CPU_burst_time > ready_queue[j].CPU_burst_time){//CPU_burst 길이가 짧을 수록 우선적으로
						tmp = ready_queue[i];
						ready_queue[i] = ready_queue[j];
						ready_queue[j] = tmp;
					}
				}
			}

			running = ready_queue[0];
			for(i=0; i<ready_count; i++){
				ready_queue[i] = ready_queue[i+1];
			}
			ready_count--;

			printf("ready queue     : ");
			for(i=0;i<ready_count;i++){
				printf("p%d  ",ready_queue[i].pid);
			}
			printf("\nrunning process : p%d\n\n", running.pid);
				
			running.start[0] = start;
			running.waiting = start - running.arrival_time;
			running.turnaround = running.waiting + running.CPU_burst_time;
			start += running.CPU_burst_time;
			running.end[0] = start;
			terminated[terminate_count] = running; 
			terminate_count++;
		}
		else{
			start++;
		}
	}
	
	evalutaion(n,terminated);
}

void preemptive_SJF(int n, Process *p){
	int i=0, j=0;
	int num=n;
	int pid=-1;
	int start=0, ready_count=0, terminate_count=0;
	int pre_ready =0, pre_termi = -1;
	Process running;
	Process sjf[20];
	Process ready_queue[20];
	Process terminated[20];
	Process tmp;

	for(i=0; i<n; i++){
		sjf[i]=p[i];
	}
	running = sjf[0];
	printf("\n====================================================================================================================\n");
	printf("Preemptive Shorest Job First Algorism\n");
	while(terminate_count != n){//모든 프로세스가 종료될 때까지 진행
		if(num!=0){ //time n일때 가상의 프로세스 도착한 것 ready_queue에 넣기
			for(i=0; i<num; i++){
				if(sjf[i].arrival_time == start){
					ready_queue[ready_count] = sjf[i];
					for(j=i;j<num;j++){
						sjf[j] =sjf[j+1];
					}
					i--;
					num--;
					ready_count++;
				}
			}
		}

		if(pre_termi == terminate_count){
			for(i=ready_count; i>0; i--){
				ready_queue[i] =ready_queue[i-1];
			}
			ready_queue[0] = running;
			ready_count++;
		}

		if(ready_count >0){	//ready queue에 프로세스 존재할 때!!
			
			if(ready_count > pre_ready){//ready_queue에 새로운 프로세스 들어왔을 때 cpu_burst_time 짧은거 확인
				for(i=0; i<ready_count-1; i++){
					for(j=i+1;j<ready_count; j++){
						if(ready_queue[i].CPU_burst_time > ready_queue[j].CPU_burst_time){
							tmp = ready_queue[i];
							ready_queue[i] = ready_queue[j];
							ready_queue[j] = tmp;
						}
					}			
				}
			}
			pre_ready = ready_count;
			pre_termi = terminate_count;

			running = ready_queue[0];
			for(i=0; i<ready_count; i++){
				ready_queue[i] = ready_queue[i+1];
			}
			ready_count--;

			if(running.pid == pid){//pid가 안변했을 경우(같은 프로세스 진행)
				for(i=0;i<20;i++){
					if(running.end[i] ==0){
						running.end[i-1]++;
						break;
					}
				}
			}
			else{//preemptive or 종료되어 새로운 pid 진행
				printf("ready queue     : ");
				for(i=0;i<ready_count;i++){
					printf("p%d  ",ready_queue[i].pid);
				}
				printf("\nrunning process : p%d\n\n", running.pid);
				
				for(i=0;i<20;i++){
					if(running.start[i] ==0&&running.end[i] ==0){
						running.start[i] = start;
						running.end[i] = start+1;
						break;
					}
				}
				if(i==0)
					running.waiting = running.start[i] - running.arrival_time;
				else
					running.waiting += running.start[i] - running.end[i-1];
			}
			pid = running.pid;
			running.CPU_burst_time--; 

			if(running.CPU_burst_time == 0){//process 종료시
				terminated[terminate_count] = running;
				for(i=0; i<n; i++){
					if(p[i].pid == running.pid){
						terminated[terminate_count].CPU_burst_time = p[i].CPU_burst_time;
						terminated[terminate_count].turnaround = terminated[terminate_count].CPU_burst_time + running.waiting;
						break;
					}
				}
				terminate_count++;
			}
		}

		start++;
	}
	evalutaion(n,terminated);
}

void Priority(int n, Process *p){
	int i=0, j=0;
	int num=n;
	int start=0, ready_count=0, terminate_count=0;
	Process running;
	Process sjf[20];
	Process ready_queue[20];
	Process terminated[20];
	Process tmp;

	for(i=0; i<n; i++){
		sjf[i]=p[i];
	}
	running = sjf[0];
	printf("\n====================================================================================================================\n");
	printf("Priority Algorism\n");
	while(terminate_count != n){//모든 프로세스가 종료될 때까지 진행
		if(num!=0){ //도착한 것 ready_queue에 넣기
			for(i=0; i<num; i++){
				if(sjf[i].arrival_time <= start){
					ready_queue[ready_count] = sjf[i];
					for(j=i;j<num;j++){
						sjf[j] =sjf[j+1];
					}
					i--;
					num--;
					ready_count++;
				}
			}
		}

		if(ready_count>0){//ready_queue에 하나 이상있을 시 running한다.
			for(i=0; i<ready_count-1; i++){
				for(j=i+1;j<ready_count; j++){
					if(ready_queue[i].priority > ready_queue[j].priority){//priority가 작을수록 우선적으로
						tmp = ready_queue[i];
						ready_queue[i] = ready_queue[j];
						ready_queue[j] = tmp;
					}
				}
			}

			running = ready_queue[0];
			for(i=0; i<ready_count; i++){
				ready_queue[i] = ready_queue[i+1];
			}
			ready_count--;

			printf("ready queue     : ");
			for(i=0;i<ready_count;i++){
				printf("p%d  ",ready_queue[i].pid);
			}
			printf("\nrunning process : p%d\n\n", running.pid);
				
			running.start[0] = start;
			running.waiting = start - running.arrival_time;
			running.turnaround = running.waiting + running.CPU_burst_time;
			start += running.CPU_burst_time;
			running.end[0] = start;
			terminated[terminate_count] = running; 
			terminate_count++;
		}
		else{
			start++;
		}
	}
	
	evalutaion(n,terminated);

}

void preemptive_Priority(int n, Process *p){
	int i=0, j=0;
	int num=n;
	int pid=-1;
	int start=0, ready_count=0, terminate_count=0;
	int pre_ready =0, pre_termi = -1;
	Process running;
	Process priority[20];
	Process ready_queue[20];
	Process terminated[20];
	Process tmp;

	for(i=0; i<n; i++){
		priority[i]=p[i];
	}
	running = priority[0];
	printf("\n====================================================================================================================\n");
	printf("Preemptive Priority Algorism\n");
	while(terminate_count != n){//모든 프로세스가 종료될 때까지 진행
		if(num!=0){ //time n일때 가상의 프로세스 도착한 것 ready_queue에 넣기
			for(i=0; i<num; i++){
				if(priority[i].arrival_time == start){
					ready_queue[ready_count] = priority[i];
					for(j=i;j<num;j++){
						priority[j] =priority[j+1];
					}
					i--;
					num--;
					ready_count++;
				}
			}
		}

		if(pre_termi == terminate_count){
			for(i=ready_count; i>0; i--){
				ready_queue[i] =ready_queue[i-1];
			}
			ready_queue[0] = running;
			ready_count++;
		}

		if(ready_count >0){	//ready queue에 있을 때!!
			
			if(ready_count > pre_ready){//ready_queue에 갯수 증가하였을 때 우선순위 확인
				for(i=0; i<ready_count-1; i++){
					for(j=i+1;j<ready_count; j++){
						if(ready_queue[i].priority > ready_queue[j].priority){
							tmp = ready_queue[i];
							ready_queue[i] = ready_queue[j];
							ready_queue[j] = tmp;
						}
					}			
				}
			}
			pre_ready = ready_count;
			pre_termi = terminate_count;

			running = ready_queue[0];
			for(i=0; i<ready_count; i++){
				ready_queue[i] = ready_queue[i+1];
			}
			ready_count--;

			if(running.pid == pid){//pid가 안변했을 경우
				for(i=0;i<20;i++){
					if(running.end[i] ==0){
						running.end[i-1]++;
						break;
					}
				}
			}
			else{//preemptive or 종료되어 새로운 pid 진행
				printf("ready queue     : ");
				for(i=0;i<ready_count;i++){
					printf("p%d  ",ready_queue[i].pid);
				}
				printf("\nrunning process : p%d\n\n", running.pid);
				
				for(i=0;i<20;i++){
					if(running.start[i] ==0&&running.end[i] ==0){
						running.start[i] = start;
						running.end[i] = start+1;
						break;
					}
				}
				if(i==0)
					running.waiting = running.start[i] - running.arrival_time;
				else
					running.waiting += running.start[i] - running.end[i-1];
			}
			pid = running.pid;
			running.CPU_burst_time--; 

			if(running.CPU_burst_time == 0){//process 종료시
				terminated[terminate_count] = running;
				for(i=0; i<n; i++){
					if(p[i].pid == running.pid){
						terminated[terminate_count].CPU_burst_time = p[i].CPU_burst_time;
						terminated[terminate_count].turnaround = terminated[terminate_count].CPU_burst_time + running.waiting;
						break;
					}
				}
				terminate_count++;
			}
		}

		start++;
	}
	evalutaion(n,terminated);
}

void RoundRobin(int n, Process *p){
	int i=0, j=0;
	int num=n;
	int quantum =0, start=0, order=0;
	int pos=0;
	int cnt =-1;
	int ready_count=0, terminate_count=0;
	Process tmp;
	Process running;
	Process rr[20];
	Process ready_queue[20];
	Process terminated[20];

	for(i=0; i<n; i++){
		rr[i]=p[i];
	}
	running = rr[0];
	printf("\n====================================================================================================================\n");
	printf("Round Robin\n");
	printf("Enter the quantum scale : ");
	scanf_s("%d",&quantum);
	
	while(terminate_count != n){
		if(num!=0){ //time n일때 도착한 것 ready_queue에 넣기
			for(i=0; i<num; i++){
				if(rr[i].arrival_time <= start){
					ready_queue[ready_count] = rr[i];
	 				for(j=i;j<num;j++){
						rr[j] =rr[j+1];
					}
					i--;
					num--;
					ready_count++;
				}
			}
		}
		for(i=0; i<ready_count; i++){//arrival_time이 먼저인 것을 먼저 저장
			for(j=i+1;j<ready_count; j++){
				if(ready_queue[i].arrival_time > ready_queue[j].arrival_time){
					tmp = ready_queue[i];
					ready_queue[i] = ready_queue[j];
					ready_queue[j] = tmp; 
				}
			}
		}
		if(cnt == terminate_count){
			ready_queue[ready_count] = running;
			ready_count++;
		}

		if(ready_count>0)//ready_queue에 process가 존재한다면
		{
			cnt = terminate_count;
			running = ready_queue[0];
			for(i=0; i<ready_count; i++){
				ready_queue[i] = ready_queue[i+1];
			}
			ready_count--;

			for(i=0;i<20;i++){
				if(running.start[i] ==0&&running.end[i] ==0){
					running.start[i] = start;
					pos = i;
					break;
				}
			}
			running.CPU_burst_time = running.CPU_burst_time - quantum;
			running.end[pos] = start + quantum;
			running.waiting += running.start[pos] - running.arrival_time;
			running.arrival_time = start + quantum; 

			if(running.CPU_burst_time <=0){	//terminated되면...
				running.end[pos] = running.end[pos]-abs(running.CPU_burst_time);
				terminated[terminate_count] = running;
				start = start - (quantum - (running.end[pos]-running.start[pos]));//terminated 될 시 start

				for(i=0; i<n; i++){
					if(p[i].pid == running.pid){
						terminated[terminate_count].arrival_time = p[i].arrival_time;
						terminated[terminate_count].CPU_burst_time = p[i].CPU_burst_time;
						terminated[terminate_count].turnaround = terminated[terminate_count].CPU_burst_time + running.waiting;
						break;
					}
				}
				terminate_count++;
			}
		}
		printf("ready queue     : ");
		for(i=0;i<ready_count;i++){
			printf("p%d  ",ready_queue[i].pid);
		}
		printf("\nrunning process : p%d\n\n", running.pid);
		
		start = start + quantum;//평상시 start는 time quantum만큼 플러스!!
	}


	evalutaion(n,terminated);
}

void evalutaion(int n, Process *p){
	int i=0, j=0;
	Process tmp;
	float total_turnaround =0;
	float total_waiting =0;
	float avg_turnaround =0;
	float avg_waiting = 0;
	float utilization=0;

	for(i=0;i<n;i++){
		total_turnaround += p[i].turnaround;
		total_waiting += p[i].waiting;
	}
	avg_turnaround = total_turnaround/n;
	avg_waiting = total_waiting/n;

	printf("After terminated\n");
	for(i=0; i<n-1; i++){//process 정렬
		for(j=i+1;j<n; j++){
			if(p[i].pid > p[j].pid){
				tmp = p[i];
				p[i] = p[j];
				p[j] = tmp;
			}
		}
	}
	for(i=0; i<n; i++){
		printf("pid: %d  waiting: %d  turnaround: %d\n",p[i].pid,p[i].waiting,p[i].turnaround);	
	}
	utilization = gantt(n ,p);

	printf("\n");
	printf("CPU utilization = %.2f\n",utilization);
	printf("average turnaround time = %.2f\n", avg_turnaround);
	printf("average waiting time = %.2f\n", avg_waiting);

}

float gantt(int n, Process *p){
	int i=0;
	int pos[20] ={0,};	
	int time=0, count=0;
	int terminal_count=0;
	int remain =0;
	float utilization =0;
	printf("\nGantt Chart!!!!");
	printf("\n|");
	while(1){		
		for(i=0; i<n; i++){
			if(time == p[i].start[pos[i]]){
				count=0;
				while(count!= p[i].end[pos[i]] - p[i].start[pos[i]]){
					if(p[i].pid <10)
						printf("p0%d|",p[i].pid);
					else
						printf("p%d|",p[i].pid);
					count++;
				}
				time = p[i].end[pos[i]];
				pos[i]++;
				break;
			}
			if(i==n-1){
				printf("xxx|");
				remain++;
				time++;
			}
		}
		
		//종료
		if(p[i].start[pos[i]] ==0  && p[i].end[pos[i]] ==0)
			terminal_count++;
		if(terminal_count ==n)
			break;
	}
	printf("\n");
	
	terminal_count =0;
	for(i=0; i<20; i++)
		pos[i]=0;
	time=0;
	while(1){
		for(i=0; i<n; i++){
			if(time == p[i].start[pos[i]]){
				if(time<10)
					printf("%d   ",time);
				else
					printf("%d  ",time);
				time++;
				while(time!=p[i].end[pos[i]]){
					printf("    ");
					time++;
				}
				pos[i]++;
				break;
			}
			if(i==n-1){
				printf("    ");
				time++;
			}
		}

		if(p[i].start[pos[i]] ==0  && p[i].end[pos[i]] ==0)
			terminal_count++;
		if(terminal_count ==n){
			printf("%d\n",time);
			utilization =(double) (time-remain)*100/time;
			return utilization;
		}

	}

}

