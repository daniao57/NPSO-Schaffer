/*
 Schaffer函数：
 f(x1,x2) = 0.5-((sin(x1^2+x2^2)^0.5)^2-0.5)/(1+0.001*(x1^2+x2^2))^2
 其中 -10 ≤ x1, x2 ≤ 10
 该函数存在无数极值点，它在 (0,0) 处存在全局最大值1，
 且在最大值附近周围存在一个圈脊，值均为0.990284，
 如果算法全局寻优能力较差则很容易陷入这个局部最优解，
 因此该函数已成为测试算法的一个标准问题，在很多研究中被采用
 新粒子群算法可以迅速的找到全局最大值（设定目标函数值超过0.9999即为找到）
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define P_NUM 200               //粒子个数
#define RANGE 10                //变量定义域
#define TEST_NUM 10             //试验次数
#define MAX_ITERATION 1000      //最大迭代次数
#define C1  0.3                 //参数
#define C2  0.6                 //参数
#define RAND01 (rand() % 32768 / 32768.0)
double _pos[P_NUM][2];
double _pbest[P_NUM][2];
double _fitness[P_NUM];
int _gbestIndex;
//目标函数计算
double calFitness(double res[2]) {
    double fitness = 0;
    double pfh = res[0] * res[0] + res[1] * res[1];
    fitness = 0.5 - (pow(sin(sqrt(pfh)), 2) - 0.5)/pow((1 + 0.001 * pfh),2);
    return fitness;
}
//变异
void mutate(double p[2]) {
    double l = RAND01 * 5;
    for (int i = 0; i < 2; i++) {
        p[i] += RAND01 * l * 2 - l;
        if (p[i] > RANGE) {
            p[i] = RANGE;
        }
        if (p[i] < -RANGE) {
            p[i] = -RANGE;
        }
    }
}
//新粒子群计算
void NPSO() {
    for (int i = 0; i < P_NUM; i++) {
        int r = RAND01;
        if (r < C1) {
            memcpy(_pos[i], _pbest[_gbestIndex], sizeof(double) * 2);
        } else if (r < C2) {
            memcpy(_pos[i], _pbest[i], sizeof(double) * 2);
        }
        mutate(_pos[i]);
    }
}
//初始化粒子
void initRes() {
    _gbestIndex = 0;
    for (int i = 0; i < P_NUM; i++) {
        _fitness[i] = -10000;
        _pos[i][0] = RANGE * RAND01 * 2 - RANGE;
        _pos[i][1] = RANGE * RAND01 * 2 - RANGE;
    }
    memcpy(_pbest, _pos, sizeof(double) * P_NUM * 2);
}
//更新函数
void updateFitness() {
    for (int i = 0; i < P_NUM; i++) {
        double fitness = calFitness(_pos[i]);
        if (fitness > _fitness[i]) {
            if (fitness > _fitness[_gbestIndex]) {
                _gbestIndex = i;
            }
            _fitness[i] = fitness;
            memcpy(_pbest[i], _pos[i], sizeof(double) * 2);
        }
    }
}
int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
    int loopNum[TEST_NUM];
    double bestFit[TEST_NUM];
    double timeUsed[TEST_NUM];
    for(int n = 0; n < 10; n++) {
        struct timeval startTime;
        gettimeofday(&startTime, NULL);
        initRes();
        updateFitness();
        for (int i = 0; i < MAX_ITERATION; i++) {
            NPSO();
            updateFitness();
            if (_fitness[_gbestIndex] > 0.9999) {
                bestFit[n] = _fitness[_gbestIndex];
                struct timeval endTime;
                gettimeofday(&endTime, NULL);
                double runningTime = endTime.tv_sec - startTime.tv_sec + (endTime.tv_usec - startTime.tv_usec) / 1000000.0;
                printf("%d\t%d\t%.6f\t%.6lf秒\n",  n + 1, i + 1, bestFit[n], runningTime);
                timeUsed[n] = runningTime;
                loopNum[n] = i + 1;
                break;
            }
        }
    }
    double avgLoopNum = 0;
    double avgBestFit = 0;
    double avgTimeUsed = 0;
    for (int t = 0; t < TEST_NUM; t++) {
        avgLoopNum += loopNum[t] * 1.0 / TEST_NUM;
        avgBestFit += bestFit[t] / TEST_NUM;
        avgTimeUsed += timeUsed[t] / TEST_NUM;
    }
    printf("平均\t%.1f\t%.6f\t%.6lf秒\n",  avgLoopNum, avgBestFit,  avgTimeUsed);
    return 0;
}
