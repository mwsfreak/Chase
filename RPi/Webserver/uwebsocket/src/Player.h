#include <string>
# include <list>
#include <iterator>

class Player
{
public:
    Player(string name);     // rettes
    void setPenalty(void);
    void setNewTime(int);
    int getPenalty(void);
    float getAVGtime(void);
    ~Player();
private:
    int index;
    string name;             // rettes
    int penalty;
    float avgTime;
    list<int> meassuredTime[];
};