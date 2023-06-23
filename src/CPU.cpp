#include"CPU.h"
#include"arithmetic.h"
#include"get_cmd.h"
using namespace std;
#define INST_CNT 2000000
#define CMD_RESULT_BUF_SIZE 4096
double time_box(Function pfun,void*funcParam)
{
	auto start = std::chrono::steady_clock::now();
	pfun(funcParam);
    auto end = std::chrono::steady_clock::now();
	double elspTime = std::chrono::duration<double,std::nano>(end - start).count();
	return elspTime;
}
void*empty_func(void*)//remove time elaps caused by function calling
{
	return NULL;
}

template<int maxiter,int height,int width>
void*mandelbrot_seralize(void*) {
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            complex<double> c(X_MIN + i * (X_MAX - X_MIN) / width, Y_MIN + j * (Y_MAX - Y_MIN) / height);
            complex<double> z(0.0, 0.0);
            int k = 0;
            while (k < maxiter && abs(z) < 2.0) {
                z = z * z + c;
                k++;
            }
            int red = k % 8 * 32;
            int green = k % 16 * 16;
            int blue = k % 32 * 8;
        }
    }
    return 0;
}

template<int thread_num,int maxiter,int height,int width>
void*mandelbrot_parallelize(void*)
{
	#pragma omp parallel for num_threads(thread_num)
	for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            complex<double> c(X_MIN + i * (X_MAX - X_MIN) / width, Y_MIN + j * (Y_MAX - Y_MIN) / height);
            complex<double> z(0.0, 0.0);
            int k = 0;
            while (k < maxiter && abs(z) < 2.0) {
                z = z * z + c;
                k++;
            }
            int red = k % 8 * 32;
            int green = k % 16 * 16;
            int blue = k % 32 * 8;
        }
    }
    return 0;
}

char percentToRank(double score)
{
	if(score>=90.0)return 'S';
	if(score>=80.0)return 'A';
	if(score>=70.0)return 'B';
	if(score>=60.0)return 'C';
	return 'D';
}

#define UPTO100(var) if(var>100)var=100;

template<int instloopcnt,int maxiter,int height,int width>
void TestDataCollection<instloopcnt,maxiter,height,width>::TestAndFillDatas()
{
	//call_func_time=time_box(empty_func,NULL);
	mandelbrot_serTime=time_box(mandelbrot_seralize<maxiter,height,width>,NULL);//-call_func_time;
	mandelbrot_paraTime[0]=time_box(mandelbrot_parallelize<testingThreadNum[0],maxiter,height,width>,NULL);//-call_func_time,
	mandelbrot_paraTime[1]=time_box(mandelbrot_parallelize<testingThreadNum[1],maxiter,height,width>,NULL);//-call_func_time,
	mandelbrot_paraTime[2]=time_box(mandelbrot_parallelize<testingThreadNum[2],maxiter,height,width>,NULL);//-call_func_time,
	mandelbrot_paraTime[3]=time_box(mandelbrot_parallelize<testingThreadNum[3],maxiter,height,width>,NULL);//-call_func_time;
	mandelbrot_paraTime[4]=time_box(mandelbrot_parallelize<testingThreadNum[4],maxiter,height,width>,NULL);//-call_func_time,
	mandelbrot_paraTime[5]=time_box(mandelbrot_parallelize<testingThreadNum[5],maxiter,height,width>,NULL);//-call_func_time,
	mandelbrot_paraTime[6]=time_box(mandelbrot_parallelize<testingThreadNum[6],maxiter,height,width>,NULL);//-call_func_time,
	mandelbrot_paraTime[7]=time_box(mandelbrot_parallelize<testingThreadNum[7],maxiter,height,width>,NULL);//-call_func_time;
	//loop_time=time_box(loop_test<instloopcnt>,NULL);
	for(int i=0;i<INST_TYPES;i++)
		inst_time[i]=time_box(instTestFunc[i],NULL);//-loop_time-call_func_time;

	oneThreadScore=100*11.5*0.9/(mandelbrot_serTime/maxiter/height/width);
	UPTO100(oneThreadScore);

	MulThreadScore=0.0;
	for(int i=0;i<thrNum_cases;i++)
		MulThreadScore+=testingThreadWeight[i]*mandelbrot_serTime/(mandelbrot_paraTime[i]*idealAccRate[i]);
	MulThreadScore/=thrNum_cases;
	MulThreadScore*=100;
	UPTO100(MulThreadScore);

	inst_score=0.0;
	for(int i=0;i<INST_TYPES;i++)
		inst_score+=inst_weight[i]*inst_basetime[i]/(inst_time[i]/instloopcnt);
	inst_score/=INST_TYPES;
	inst_score*=100;
	UPTO100(inst_score);
}
template<int instloopcnt,int maxiter,int height,int width>
void TestDataCollection<instloopcnt,maxiter,height,width>::outputReport()
{
		printf(
"CPU performance report\n\
------------------------\n\
");
		//elaps Time
		printf(
"1.CPU information\n\
");

		char result[CMD_RESULT_BUF_SIZE]={};
		getCPUinformation(result);
		printf("%s\n",result);

		printf(
"\
2. Mandelbrot test\n\
	Problem scale:MAX_ITER %d,HEIGHT %d,WIDTH %d\n\
	One thread elapsed time:%.3f ns,\n\
	Elapsed time(ns)/(MAX_ITER*HEIGHT*WIDTH)=%.3f\n\n",MAX_ITER,HEIGHT,WIDTH,
				mandelbrot_serTime,
				mandelbrot_serTime/(maxiter*height*width));
		printf(
"	Performance on multithreads:\n\
	---------------------------------------------------------------\n\
	Thread num |%17s | elaps time(ns)/(MAX_ITER*HEIGHT*WIDTH) | accelerate rate\n" ,"elaps time(ns)");
		for(int i=0;i<thrNum_cases;i++)
		{
			printf(
"	%7d    |%17.3lf | %19.3lf                    | %7.3lf\n",
			testingThreadNum[i],mandelbrot_paraTime[i],
			mandelbrot_paraTime[i]/(maxiter*height*width),
			mandelbrot_serTime/mandelbrot_paraTime[i]
			);
		}

		printf("\n\
	Single thread score:%.2lf\n\
		Rank(S/A/B/C/D):%c\n\n",oneThreadScore,percentToRank(oneThreadScore));

		printf("\
	Multi thread accelerate score:%.2lf\n\
		Rank(S/A/B/C/D):%c",MulThreadScore,percentToRank(MulThreadScore));

		double t1 = matrix_mul();
		double s1 = get_score(0,t1);
		char r1 = get_rank(s1);

		double t2 = conv_2D();
		double s2 = get_score(1,t2);
		char r2 = get_rank(s2);

		double t3 = Simpson_Integral();
		double s3 = get_score(2,t3);
		char r3 = get_rank(s3);

		double t4 = FFT();
		double s4 = get_score(3,t4);
		char r4 = get_rank(s4);

		printf(
"\n\n\
3.Common Arithmetic test\n");
		printf(
"\
	Matrix Multiplication(512x512 512x512):\n\
	elapsed time(s):%.3f\n\
	Score:%.3f\n\
	Rank(S/A/B/C/D):%c\n\
",t1,s1,r1);

		printf(
"\n\
	Two-dimensional Convolution(1024x1024 Core:5x5):\n\
	elapsed time(s):%.3f\n\
	Score:%.3f\n\
	Rank(S/A/B/C/D):%c\n\
",t2,s2,r2);

		printf(
"\n\
	Simpson Integral(partition 1e7):\n\
	elapsed time(s):%.3f\n\
	Score:%.3f\n\
	Rank(S/A/B/C/D):%c\n\
",t3,s3,r3);

		printf(
"\n\
	FFT(262144): \n\
	elapsed time(s):%.3f\n\
	Score:%.3f\n\
	Rank(S/A/B/C/D):%c\n\
",t4,s4,r4);


		printf(
"\n\n\
4.Instruction test\n\
	Instruction repeat time:%d\n\
	Instruction average elapsed time:\n\
	---------------------------------\n\
	Instruction  |  Mean elapsed time(ns)\n",instloopcnt);

	for(int i=0;i<INST_TYPES;i++)
	{
		printf("\
	%11s  |  %.3lf\n",inst_name[i],inst_time[i]/instloopcnt);
	}
	printf("\n");
			
		printf("\
	Instruction score:%.2lf\n\
		Rank(S/A/B/C/D):%c",inst_score,percentToRank(inst_score));
		printf("\n\n");
}
void InstrTestMain()//Call this
{
	TestDataCollection<1000000,MAX_ITER,HEIGHT,WIDTH>tdc;
	//printf("peaceful beginning\n");
	tdc.TestAndFillDatas();
	freopen("./result/CPU_report.txt","w",stdout);
	tdc.outputReport();
	freopen("/dev/tty", "w", stdout);
	//printf("peaceful ending\n");
}
// int main()
// {
// 	InstrTestMain();
// 	return 0;
// }