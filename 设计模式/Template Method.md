> app.cpp  
```cpp
class Application : public Library {
protected:
	virtual bool Step2(){
    }

    virtual void Step4() {
    }
};




int main()
	{
	    Library* pLib=new Application();
	    lib->Run();

		delete pLib;
	}

```

> lib.cpp
```cpp
class Library{
public:
    void Run(){
        
        Step1();

        if (Step2()) { 
            Step3(); 
        }

        for (int i = 0; i < 4; i++){
            Step4(); 
        }

        Step5();

    }
	virtual ~Library(){ }

protected:
	
	void Step1() { 
    }
	void Step3() {//�ȶ�
    }
	void Step5() { //�ȶ�
	}

	virtual bool Step2() = 0;
    virtual void Step4() =0; 
};
```

整个运行流程应该是是在框架中定义的，需要用户自定义的代码定义为纯虚函数