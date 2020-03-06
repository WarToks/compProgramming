#include <iostream>
#include <string>
#include <chrono>

/* 時間計測
    括弧でProcessingTimeをローカル変数にして, 以下のようにして時間計測を行う
    {
        ProcessingTime Pt;
        // 計測したいコード
    }
*/
class  ProcessingTime{
private:
    const std::string name;
    const std::chrono::system_clock::time_point start;
public:
    ProcessingTime(const std::string&& name = "Process"): 
                    name(std::move(name)), start(std::chrono::system_clock::now()){
    }
    ~ProcessingTime(void){
        const auto end = std::chrono::system_clock::now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds >(end - start).count();
        std::cout << name << " : " << elapsed << " ms\n";
    }
};