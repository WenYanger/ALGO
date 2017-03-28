#include <QCoreApplication>
#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;



void y_initiateGroups();
void y_travelsal();
double y_fitness(int n);
void y_pickChroms();
void y_crossover();
void y_mutation();

const int y_totalNodeNumber = 800; //每个个体中的基因节点个数，也就是节点数目
const int y_totalGroupNumber = 4; //种群个体个数，如为n，表示当前种群中有n个个体参与繁衍和变异

typedef struct Chrom                           // 结构体类型，为单个染色体的结构；
{
    short int bit[y_totalNodeNumber];//一共6bit来对染色体进行编码，其中1位为符号位。取值范围-64~+64
    int fit ;//适应值
    double rfit;//相对的fit值，即所占的百分比
    double cfit;//积累概率
}chrom;
chrom y_groupCurrent [y_totalGroupNumber];          // 初始种群规模
chrom y_groupNext    [y_totalGroupNumber];          // 更新后种群规模


//初始化种群
void y_initiateGroups()
{
    double sum = 0.0;
    srand(clock() + (unsigned)time(NULL));
    for(int i=0; i<y_totalGroupNumber; i++){
        for(int j=0; j<y_totalNodeNumber; j++){
            y_groupCurrent[i].bit[j] = (short)(rand() % 2);
        }
    }
    for(int i=0; i<y_totalGroupNumber; i++){
        y_groupNext[i] = y_groupCurrent[i];
        y_groupCurrent[i].fit = y_fitness(i);
        sum = sum + y_groupCurrent[i].fit;
    }
    //计算适应值得百分比，该参数是在用轮盘赌选择法时需要用到的
    for (int i=0; i<y_totalGroupNumber; i++)
    {
        y_groupCurrent[i].rfit = y_groupCurrent[i].fit/sum;
        y_groupCurrent[i].cfit = 0;//将其初始化为0
    }
}

//从种群中选出若干个体进行交叉、变异，这里采用排序法来选择，即每次选择都选出适应度最高的两个个体
void y_pickChroms()
{
    int i ,j;
    chrom temp ;                                // 中间变量
    int top = y_totalGroupNumber-1;
    for(i=0; i<top; i++)                          // 根据个体适应度来排序；（冒泡法）
    {
        for(j=0; j<top-i; j++)
        {
            if(y_groupNext[j+1].fit > y_groupNext[j].fit)
            {
                temp             = y_groupNext[j+1];
                y_groupNext[j+1] = y_groupNext[j];
                y_groupNext[j]   = temp;
            }
        }
    }
}

void y_crossover()
{
    srand(clock() + (unsigned)time(NULL));
    int random = rand() % y_totalNodeNumber;          // 随机产生交叉点,交叉点控制在0到y_totalGroupNumber之间；
    int i ;
    for(i=0; i<random; i++)
    {
        y_groupNext[2].bit [i]= y_groupNext[0].bit [i];   // child 1 cross over
        y_groupNext[3].bit [i]= y_groupNext[1].bit [i];   // child 2 cross over
    }

    for(i=random; i<y_totalNodeNumber; i++)         // crossing the bits beyond the cross point index
    {
        y_groupNext[2].bit [i]= y_groupNext[1].bit [i];    // child 1 cross over
        y_groupNext[3].bit [i]= y_groupNext[0].bit [i];    // chlid 2 cross over
    }

    for(i=0; i<y_totalGroupNumber; i++)
    {
        y_groupNext[i].fit= y_fitness(i);        // 为新个体计算适应度值；
    }
}

void y_mutation()
{
    int row ,col;
    int random = rand ()%50;  // 随机产生到之间的数；
    //变异操作也要遵从一定的概率来进行，一般设置为0到0.5之间
    if(random >40)                              // random==25的概率只有2%，即变异率为，所以是以小概率进行变异！！
    {
        col=rand()%y_totalNodeNumber;          // 随机产生要变异的基因位号；
        row=rand()%y_totalGroupNumber;         // 随机产生要变异的染色体号；

        if(y_groupNext[row]. bit[col]==0)             // 1变为；
        {
            y_groupNext[row].bit[col]=1 ;
        }
        else if (y_groupNext[row].bit[col]==1)        // 0变为；
        {
            y_groupNext[row].bit[col]=0;
        }
        y_groupNext[row].fit= y_fitness(row);     // 计算变异后的适应度值；
    }
}

//输出当前的种群
void y_travelsal()
{
    for(int i=0; i<y_totalGroupNumber; i++)
    {
        for(int j=0; j<y_totalNodeNumber; j++)
        {
            cout<<y_groupCurrent[i].bit[j]<<" ";
        }
        cout<<y_groupCurrent[i].fit<<endl;
    }
}
//输出下一代种群
void y_travelsal_next()
{
    for(int i=0; i<y_totalGroupNumber; i++)
    {
        for(int j=0; j<y_totalNodeNumber; j++)
        {
            cout<<y_groupNext[i].bit[j]<<" ";
        }
        cout<<y_groupNext[i].fit<<endl;
    }
}


//传入行号，对y_groupCurrent的第n行进行适应度计算，返回适应度数值
double y_fitness(int n)
{
    int sum = 0;
    for(int i=0; i<y_totalNodeNumber; i++)
    {
//        if(y_groupNext[n].bit[i] == 1){
//            sum += pow(2,i);
//        }
        sum += y_groupNext[n].bit[i];
    }
    return (double)sum;
}

//开始迭代
void y_startIteration(int num) //num为迭代次数
{
    for(int i=0; i<num; i++)                          // 开始迭代；
    {
        //cout<<"itr: "<<i<<endl;                 // 输出当前迭代次数；
        for(int j=0; j<y_totalGroupNumber; j++)
        {
            y_groupNext[j]=y_groupCurrent[j];           // 更新种群；
        }


        y_pickChroms();                    // 挑选优秀个体；
        //y_travelsal_next();
        y_crossover();                     // 交叉得到新个体；
        y_mutation();                      // 变异得到新个体；

        for(int j=0; j<y_totalGroupNumber; j++)
        {
            y_groupCurrent[j]=y_groupNext[j];              // 种群更替；
        }

        //cout<<y_groupCurrent[0].fit<<endl;

    }  // 等待迭代终止；
}

void y_startIterationByTime(int timeLimit)
{
    int starttime = clock();
    int curtime = clock();
    while(1)
    {
        if(curtime - starttime >= timeLimit)
            break;

        for(int j=0; j<y_totalGroupNumber; j++)
        {
            y_groupNext[j]=y_groupCurrent[j];           // 更新种群；
        }


        y_pickChroms();                    // 挑选优秀个体；
        //y_travelsal_next();
        y_crossover();                     // 交叉得到新个体；
        y_mutation();                      // 变异得到新个体；

        for(int j=0; j<y_totalGroupNumber; j++)
        {
            y_groupCurrent[j]=y_groupNext[j];              // 种群更替；
        }
        curtime = clock();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    y_initiateGroups();
    cout<<y_groupCurrent[0].fit<<endl;
    //y_startIteration(5000000);
    y_startIterationByTime(20000);


    cout<<y_groupCurrent[0].fit;


    return a.exec();
}
