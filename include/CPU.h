#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include<cstdio>
#include<chrono>
#include<string>
#include<complex>
#include<fstream>
#include<numeric>
#include<omp.h>
const double X_MIN = -2.0;  // x min
const double X_MAX = 1.0;   // x max
const double Y_MIN = -1.5;  // y min
const double Y_MAX = 1.5;   // y max
const int MAX_ITER = 1000;  //
const int HEIGHT = 600;     // image height
const int WIDTH = 800;      // image width
typedef void*Function(void*);
double time_box(Function pfun,void*funcParam);
void*empty_func(void*);

template<int maxiter,int height,int width>
void*mandelbrot_seralize(void*);
template<int thread_num,int maxiter,int height,int width>
void*mandelbrot_parallelize(void*);

#define DECL_INST_TEST_FUNC(func_name,test_inst) \
template<int instcnt>\
void*func_name(void*)\
{\
	for(int i=0;i<instcnt;i++)\
		asm(test_inst);\
	return NULL;\
}
constexpr char*inst_name[]=
{
	"ADD",
	"SUB",
	"MUL",
	"SDIV",
	"UDIV",

	"FABS",
	"FNEG",
	"FSQRT",
	"FADD",
	"FSUB",
	"FDIV",
	"FMUL",
	"FNMUL",
	"FMADD",
	"FMSUB",

	"FABS_SIMD",
	"FNEG_SIMD",
	"FSQRT_SIMD",
	"FADD_SIMD",
	"FSUB_SIMD",
	"FDIV_SIMD",
	"FMUL_SIMD"
};
DECL_INST_TEST_FUNC(loop_test,"")
//5
DECL_INST_TEST_FUNC(test_add,"add x0,x1,x2")
DECL_INST_TEST_FUNC(test_sub,"sub x0,x1,x2")
DECL_INST_TEST_FUNC(test_mul,"mul x0,x1,x2")
DECL_INST_TEST_FUNC(test_sdiv,"sdiv x0,x1,x2")
DECL_INST_TEST_FUNC(test_udiv,"udiv x0,x1,x2")
//floats 10
DECL_INST_TEST_FUNC(test_fabs,"fabs s0,s1")
DECL_INST_TEST_FUNC(test_fneg,"fneg s0,s1")
DECL_INST_TEST_FUNC(test_fsqrt,"fsqrt s0,s1")
DECL_INST_TEST_FUNC(test_fadd,"fadd s0,s1,s2")
DECL_INST_TEST_FUNC(test_fsub,"fsub s0,s1,s2")
DECL_INST_TEST_FUNC(test_fdiv,"fdiv s0,s1,s2")
DECL_INST_TEST_FUNC(test_fmul,"fmul s0,s1,s2")
DECL_INST_TEST_FUNC(test_fnmul,"fnmul s0,s1,s2")
DECL_INST_TEST_FUNC(test_fmadd,"fmadd s0,s1,s2,s3")
DECL_INST_TEST_FUNC(test_fmsub,"fmsub s0,s1,s2,s3")
//SIMD 7
DECL_INST_TEST_FUNC(test_fabs_SIMD,"fabs v0.4s,v1.4s")
DECL_INST_TEST_FUNC(test_fneg_SIMD,"fneg v0.4s,v1.4s")
DECL_INST_TEST_FUNC(test_fsqrt_SIMD,"fsqrt v0.4s,v1.4s")
DECL_INST_TEST_FUNC(test_fadd_SIMD,"fadd v0.4s,v1.4s,v2.4s")
DECL_INST_TEST_FUNC(test_fsub_SIMD,"fsub v0.4s,v1.4s,v2.4s")
DECL_INST_TEST_FUNC(test_fdiv_SIMD,"fdiv v0.4s,v1.4s,v2.4s")
DECL_INST_TEST_FUNC(test_fmul_SIMD,"fmul v0.4s,v1.4s,v2.4s")


#define thrNum_cases 8
constexpr int testingThreadNum[thrNum_cases]={2,4,8,16,32,64,128,256};
constexpr double idealAccRate[thrNum_cases]={2,3,4,6,10,17,32,53};
constexpr double testingThreadWeight[thrNum_cases]={1.75,1.5,0.5,0.25,1,1,1,1};
#define INST_TYPES 22
constexpr double inst_basetime[INST_TYPES]=
{2.61,2.61,2.7,3.15,3.15,
2.7,2.7,2.7,2.7,2.7,
2.7,2.7,2.7,2.7,2.7,
2.7,2.7,2.7,2.7,2.7,
2.7,2.7};
constexpr double inst_weight[INST_TYPES]=
{2.0,1.4,0.8,0.4,0.4,
1,1,1,1,1,
1,1,1,1,1,
1,1,1,1,1,
1,1};
char percentToRank(double score);
void InstrTestMain();//Call this

template<int instloopcnt,int maxiter,int height,int width>
class TestDataCollection
{
	private:
		double mandelbrot_serTime;
		double mandelbrot_paraTime[thrNum_cases];
		double inst_time[INST_TYPES];
		double MulThreadScore;
		double oneThreadScore;
		double inst_score;
		//double call_func_time;
		//double loop_time;
		const Function *instTestFunc[INST_TYPES]=
		{
			test_add<instloopcnt>,
			test_sub<instloopcnt>,
			test_mul<instloopcnt>,
			test_sdiv<instloopcnt>,
			test_udiv<instloopcnt>,

			test_fabs<instloopcnt>,
			test_fneg<instloopcnt>,
			test_fsqrt<instloopcnt>,
			test_fadd<instloopcnt>,
			test_fsub<instloopcnt>,
			test_fdiv<instloopcnt>,
			test_fmul<instloopcnt>,
			test_fnmul<instloopcnt>,
			test_fmadd<instloopcnt>,
			test_fmsub<instloopcnt>,

			test_fabs_SIMD<instloopcnt>,
			test_fneg_SIMD<instloopcnt>,
			test_fsqrt_SIMD<instloopcnt>,
			test_fadd_SIMD<instloopcnt>,
			test_fsub_SIMD<instloopcnt>,
			test_fdiv_SIMD<instloopcnt>,
			test_fmul_SIMD<instloopcnt>
		};
	public:
	void TestAndFillDatas();
	void outputReport();
};

#endif