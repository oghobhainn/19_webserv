#include "../main.hpp"

template<class T>
void show_list_str(T & l)
{
	std::cout << "--List string : " << std::endl;

	for (typename T::iterator it = l.begin(); it != l.end(); it++)
		std::cout << *it << std::endl;;
	std::cout << "--END" << std::endl;
}

template<class T>
void show_list(T & l)
{
	std::cout << "--List class : " << std::endl;;
	for (typename T::iterator it = l.begin(); it != l.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "--END" << std::endl;
}

std::vector<std::string> split(std::string s, const char delim) 
{
    std::vector<std::string>	result;
    std::stringstream			ss(s);
    std::string					item;

    while (getline(ss, item, delim)) 
        result.push_back(item);
    return result;
}

int len(std::string str)  
{  
    int length = 0; 

    for (int i = 0; str[i] != '\0'; i++)  
        length++;
    return length;     
}

std::list<std::string> split_lst(std::string str, char seperator)  
{
    int i = 0;  
    int startIndex = 0, endIndex = 0;
    std::list<std::string>      result;

    while (i <= len(str))  
    {  
        if (str[i] == seperator || i == len(str))  
        {  
            endIndex = i;  
            std::string subStr = "";
            subStr.append(str, startIndex, endIndex - startIndex);
            if (len(subStr) > 0)
                result.push_back(subStr);
            startIndex = endIndex + 1;  
        }
        i++;  
    }
    return result; 
}

std::string trim(std::string str, std::string whitespace)
{
    unsigned long strBegin = str.find_first_not_of(whitespace);
    unsigned long strEnd = str.find_last_not_of(whitespace);
    unsigned long strRange = strEnd - strBegin + 1;

    if (strBegin == std::string::npos)
        return "";
    return str.substr(strBegin, strRange);
}

int countFreq(std::string pat, std::string txt)
{
    int M = pat.length();
    int N = txt.length();
    int res = 0;
   
    for (int i = 0; i <= N - M; i++)
    {
        int j;
        for (j = 0; j < M; j++)
            if (txt[i + j] != pat[j])
                break;
        if (j == M) 
        {
           res++;
           j = 0;
        }
    }
    return res;
}

int nthOccurrence(const std::string& str, const std::string& findMe, int nth)
{
    size_t  pos = 0;
    int     cnt = 0;

    while (cnt != nth)
    {
        pos += 1;
        pos = str.find(findMe, pos);
        if ( pos == std::string::npos )
            return -1;
        cnt++;
    }
    return pos;
}