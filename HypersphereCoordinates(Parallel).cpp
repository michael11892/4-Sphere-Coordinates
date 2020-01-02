#include<iostream>
#include<fstream>
#include<cmath>
#include<thread>
#include<string>
#include<stdio.h>

using namespace std;

int ThreadCount=0;

void threadFunction(float Xmin,float Xmax,float Ymin,float Ymax,float Zmin,float Zmax,float Wmin,float Wmax,float R,float p)
{
	int ThreadID=ThreadCount;
	ThreadCount++;
	ofstream fout("Hypersphere(Parallel)"+to_string(ThreadID)+".txt");
	float x,y,z,w,D; //minimum value of x y z w and Constant value of R
    float progressPercent;
    long long int progress=0,progressMax=(Xmax-Xmin)*(Ymax-Ymin)*(Zmax-Zmin)*(Wmax-Wmin)/(p*p*p*p);
    for(w=Wmin;w<=Wmax;w=w+p)
    {
        for(z=Zmin;z<=Zmax;z=z+p)
        {
            for(y=Ymin;y<=Ymax;y=y+p)
            {
                for(x=Xmin;x<=Xmax;x=x+p)
                {
                    progress++;
                    progressPercent=(float)progress/progressMax;
                    //cout<<progressPercent*100<<"%"<<"            "<<"\r";
                    D=sqrt(x*x + y*y + z*z + w*w -R*R);
                    D=D/p;
                    D=round(D);
                    D=D*p;
                    if(D==0)
                    {
                    	cout<<round(x/p)*p<<" "<<round(y/p)*p<<" "<<round(z/p)*p<<" "<<round(w/p)*p<<endl;
                        fout<<round(x/p)*p<<" "<<round(y/p)*p<<" "<<round(z/p)*p<<" "<<round(w/p)*p<<endl;
                    }
                }
            }
        }
    }
    fout.close();
}

int main()
{
    int ThreadMax=10;
    float Xmin,Xmax,Ymin,Ymax,Zmin,Zmax,Wmin,Wmax,R,p;
    cout<<"Minimum and Maximum Values for X,Y,Z,W and the values for R, maximum Precision and Threads to be used:"<<endl;
    cin>>Xmin>>Xmax>>Ymin>>Ymax>>Zmin>>Zmax>>Wmin>>Wmax>>R>>p>>ThreadMax; //example input -1 1 -1 1 -1 1 -1 1 1 0.01 10
    float Xdiff = (Xmax-Xmin)/ThreadMax;

    //Creating the independent threads
    thread t[ThreadMax];
    for(int i=0;i<ThreadMax;i++)
    {
        t[i]=thread(threadFunction,Xmin,Xmin+Xdiff,Ymin,Ymax,Zmin,Zmax,Wmin,Wmax,R,p);
        Xmin=Xmin+Xdiff;
    }

    //Closing the independent threads
    for(int i=0;i<ThreadMax;i++)
    {
        t[i].join();
    }

    ofstream fout("Hypersphere(Parallel).txt");
    for(int i=0;i<ThreadCount;i++) //Combine the data from all the temporary files into the primary output
    {
    	ifstream in("Hypersphere(Parallel)"+to_string(i)+".txt");
    	while(!in.eof())
    	{
    		string temp;
    		getline(in,temp);
    		fout<<temp<<endl;
    	}
    	in.close();
    }
    fout.close();

    for(int i=0;i<ThreadCount;i++) //Cleanup all the temporary files
    {
    	string fname("Hypersphere(Parallel)"+to_string(i)+".txt");
    	if(remove(fname.c_str())==0)
    	{
    		cout<<"Successfully cleaned up: "<<"Hypersphere(Parallel)"+to_string(i)+".txt"<<endl;
    	}else{
    		cout<<"Failed to clean up: "<<"Hypersphere(Parallel)"+to_string(i)+".txt"<<endl;
    	}
    }

    return 0;
}
