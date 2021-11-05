
//  Created by Ömer faruk Yakşi on 5.11.2021.
//

#include <iostream>
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <string>

using namespace std;
/*
 Firstly there is string function getnumber that used for getting input from user and prevent wrong input before program goes to next step.
 
 */
string getnumber(){
    int num=0;
    string buffer;
    
start:
    //    Creating buffer string and waiting for user input
    getline(cin,buffer,'\n');
    
    //    Checking buffer input for if there is any character except numbers and also checking for length because there must be 4 digit.
    if( buffer.find_first_not_of("0123456789") == string::npos  && buffer.length()<5 && buffer.length()>3   )
    {
        num= stoi(buffer);
    }
    
    else{
        cout<<"Enter valid integer with 4 digit "<<endl;
        goto start;
        
    }
    
    
    return to_string(num);
}





/*
 
 count_tp means counter for true positives. As in the game cows and bulls program scans for true positives in secret number
 */

int count_tp(string secret,string guess,bool digits_used_secret[],bool digits_used_guess[]){
    int count=0;
    for (int i = 0; i < 4; i++) {
        if (secret[i] == guess[i]) {
            count++;
            // Mark digits used
            digits_used_secret[i] = true;
            digits_used_guess[i] = true;
        }
    }
    
    
    return count;
}

//Count_fp means counter for false positives. Meaning number has this figure but not in correct place.


int count_fp(string secret,string guess,bool digits_used_secret[],bool digits_used_guess[]){
    int count=0;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // Don't use an alredy used digit
            if (digits_used_secret[i] || digits_used_guess[j])
                continue;
            
            if (secret[i] == guess[j]) {
                count++;
                // Mark digits used
                digits_used_secret[i] = true;
                digits_used_guess[j]  = true;
            }
        }
    }
    
    
    
    
    return count;
    
}

/* Calculating the score of computer's guess from users input. Firstly checking if the input has false positive and calculates how many figures founded in secret number
 */

int calcscore(string feedback){
    
    int score;
    
    if(feedback.size()<3 ) score = feedback[0]-'0';
    else score = (feedback[2]-'0') + (feedback[0]-'0');
    
    return score;
    
}







int main(void){
    
    /*
     
     Firstly setting seed to time dependent so every time user opened program it will give different 4 digit number.
     Then creating array for marking the numbers so if a figure is not in place then program wont show it again.
     
     */
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    bool used_numbers[10] = {false,false,false,false,false,false,false,false,false,false};
    vector<int> digits { 0,1,2,3,4,5,6,7,8,9};
    vector<int> digits_guess {9,8,7,6,5,4,3,2,1,0};
    
    
    //    shuffling for both secret number and initial guess for computer.
    shuffle (digits_guess.begin(), digits_guess.end(), std::default_random_engine(seed));
    shuffle (digits.begin(), digits.end(), std::default_random_engine(seed));
    
    
    /*
     
     After the shuffling done. We can initialize our secret number and initial guess for computer. To do so, Checking if the first element is zero and if it is taking last element of shuffled array.
     Then whether first index is 0 or not filling our number and guess from index i=1 to 3.
     
     */
    unsigned int dummy=0;
    unsigned int dummyg=0;
    if( digits_guess[0]!=0) dummyg=digits_guess[0];
    else
        dummyg=digits_guess[9];
    if(digits[0]!=0) dummy= digits[0];
    
    else dummy= digits[9];
    
    used_numbers[dummyg]=true;
    
    for (int i=1; i<4;i++){
        dummy*=10;
        dummy+=digits[i];
        dummyg*=10;
        dummyg+=digits_guess[i];
        used_numbers[digits_guess[i]]=true;
        
    }
    //    After secret number and initial guess created converting these number to string in order to manipulate them.
    string comp_guess=to_string(dummyg);
    string secret= to_string(dummy);
    /*
     
     Keeping track of scores and previous scores(oldscore), number we used on computers guess and previous number used in order to
     If initial guess last digit is on users secret number. We can recover it from this variable.
     
     */
    
    int oldscore=0;
    int score=0;
    int ind=3;
    int number=0;
    int oldnumber=0;
    int counter=0;
    int old_tp=0;
    cout<< " Enter 4 digit unique number to guess "<<endl;
    string temp_tp;
    int lists[4];
    int count=0;
    int change=1;
    int comp_tp=0;
    
    while(1){
        
        
        string guess=getnumber();
        
        
        bool secret_digit_used[4] = {false, false, false, false};
        bool guess_digit_used[4]  = {false, false, false, false};
        
        
        
        
        int guest_tp= count_tp(secret, guess, secret_digit_used, guess_digit_used);
        
        
        int guest_fp=count_fp(secret, guess, secret_digit_used,  guess_digit_used);
        guest_fp= -guest_fp;
        
        
        /*
         Checking users  guess and scoring it after scoring is done if user or computer has found exact number program will quit.
         
         */
        
        
        
        if( guest_tp ==4 )
        {
            cout<<"Congrulations you found number before computer "<<endl;
            break;
            
        }
        
        else if ( comp_tp==4)
        {
            cout<<"Computer found your number you lost"<<endl;
            break;
        }
        
        
        
        /*
         
         This if is set to score==4 because there are 2 way to calculate the number user holds.
         1- Find the all digits in number and then check each permutation.
         2- Use Knuth's algorithm and keep backtracking until number reached.
         Knuth's algorithm is far more superior and faster but since i can only implement first method;
         Until all digits has found try each digit.
         
         
         */
        if(score != 4 ) {
            
            cout<<"Your feedback is: +"<<guest_tp<<" "<< guest_fp<<endl;
            
            cout<<" Computers guess: " << comp_guess<<endl;
            
            
            cout<<"Enter feedback for my guess"<<endl;
            oldscore=score;
            getline(cin,temp_tp,'\n');
            old_tp=comp_tp;
            comp_tp=temp_tp[0] -'0';
            score=calcscore(temp_tp);
            /*
             Recording previous score in order to compare and check if program passed correct value
             
             If score is increased, this means last number we used is in the number so we change our digit to check other numbers.
             Also there is variable named count that used for if program passed true value after first initialize in example
             initial guess is 1234 and users secret is 5904. After user enter feedback no matter what is the feedback last digit will change in order to compare states and find true values. So to prevent this action there is count variable after the first 2 try computer will change indice.
             
             */
            if( score>oldscore && count>0){
                
                ind--;
                change=1;
                
            }
            else if( score<oldscore && count>0 ){
                char s;
                s=oldnumber+'0';
                comp_guess[ind]=s;
                used_numbers[number]=1;
                change=0;
                
            }
            else if ( score== oldscore){
                change=1;
            }
            
            if(score==4) change=0;
            if( ind<0) ind=3;
            if(change>0){
                for ( int i=0;i<10; i++){
                    if( used_numbers[i]==0  ){
                        
                        //                        In order to compare int with char value transforming int to char.
                        oldnumber=comp_guess[ind]-'0';
                        number=i;
                        char s;
                        s=number+'0';
                        comp_guess[ind]=s;
                        used_numbers[i]=1;
                        break;
                    }
                    
                }
            }
            
            
            
            
            count++;
        }
        
        else if ( score==4 && counter==0){
            counter++;
            /*
             After program founds all 4 digits there is no need to use string anymore because program will not change with external value.
             Also in order to produce the all permutations of number it is transformed to array
             */
            lists[0]= comp_guess[0]-'0';
            lists[1]= comp_guess[1]-'0';
            lists[2]= comp_guess[2]-'0';
            lists[3]= comp_guess[3]-'0';
            sort(begin(lists),end(lists));
            cout<<"Your feedback is: +"<<guest_tp<<" "<< guest_fp<<endl;
            cout<< "Computer guess:  "<<lists[0]<<lists[1]<<lists[2]<<lists[3]<<endl;
            cout<< "Enter  feedback: "<<endl;
            getline(cin,temp_tp,'\n');
            comp_tp=temp_tp[0] -'0';
            score=calcscore(temp_tp);
            
            ind=3;
            
        }
        else if (score==4 && counter>0){
            /*            After transforming to array no matter what true positive or false postive user entered program will just produce next permutation */
            
            cout<<"Your feedback is: +"<<guest_tp<<" "<< guest_fp<<endl;
            cout<< "Computer guess:  "<<lists[0]<<lists[1]<<lists[2]<<lists[3]<<endl;
            cout<< "Enter  feedback: "<<endl;
            
            getline(cin,temp_tp,'\n');
            comp_tp=temp_tp[0] -'0';
            
            std::next_permutation(lists,lists+4);
            
            
            
        }
        
        
        
        
        
        
        cout<<"Enter New Guess"<<endl;
        
    }
    
    
    
}
