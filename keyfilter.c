#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LEN_LINE 101
#define ASCII_LIMIT 127

//Funkce seřadí řetězec podle abecedy
void alphaSort(char* str, char* sortedStr){
   int len = strlen(str); 
   int min = ASCII_LIMIT; //nejvyšší index v základní ascii tabulky
   int index;

   for (int i = 0; i < len; i++){
       for (int j = 0; j < len; j++){
         if (str[j] < min){
            min = str[j]; //jako aktuální minimum se uloží znak v řetězci
            index = j; //zaznamenáme jeho index do proměnné index
         }
      }
      sortedStr[i] = min; //vložíme minimum nalezené v neseřazeném na pozici jemu náležící 
      str[index] = ASCII_LIMIT; //nahradíme minimum nalezené v neseřazeném řetěci znakem 127 aby se již znovu nestal minimem
      min = ASCII_LIMIT; 
   }
}
     
// Funkce zjistí zda znak již není v řetězci
int duplicateCheck(char* str, char c){
   int len = strlen(str);
   int duplicateFlag = 0;

   for (int i = 0; i < len; i++){
      if (c == str[i]){ // pokud se znak rovná znaku v řetěci nastaví duplcateFlag na 1
         duplicateFlag = 1;
      }
   }
   if (duplicateFlag == 0){ //pokud je není duplicateFlag 0 (což by znamenalo že kontrolovany znak v řetězci není vrať 1) 
      return 1;

   } else {
      return 0;
   }
}

//Funkce zjistí zda je v řetězci mezera
int spaceCheck(char* str){
   int len = strlen(str);
   int check;
   for (int i = 0; i < len; i++){
      check = isspace(str[i]);
      if (check){
         return 1;
      }
   }
   return 0;
}

//Funkce změní všechny písmena v řetěžci na velká
void allCaps(char* str) {
   for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]); //pro každý znak v řetězci prověď funkci toupper
   } 
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

int main(int argc, char **argv) {

   char sortedEnabledChars[LEN_LINE] = "";
   char buffer[LEN_LINE + 1];
   char uInput[LEN_LINE] = "";
   char matches[LEN_LINE];
   char enabledChars[LEN_LINE] = "";
   int matchedChars = 0; 
   int lenInput;
   int matchCount = 0;
   int lenLine;
   int enCharIndex = 0;

   //----------------------------------------------------------------
   //Logika pro případ kdy uživatel nezadá nic z klávesnice (program vypíše první písmena všech řádků)
   // Při každém průchodu cyklem se uloží první písmeno řádku získaného funkcí fgets do enableChars pole

   if (argc < 2) {
      while (fgets(buffer, sizeof(buffer), stdin) != NULL) { //cyklus pokračuje dokud nenajde řádek ve kterém nejsou data (NULL)
         lenLine = strlen(buffer);
         if (lenLine > 100){
            fprintf(stderr, "ERROR: Line exceeded 100 chars.");
            return 1;
         }
         allCaps(buffer);
         if (duplicateCheck(enabledChars, buffer[0]) == 1){
            enabledChars[matchCount] = buffer[0];
            matchCount++;
         }
      }
      alphaSort(enabledChars, sortedEnabledChars);
      printf("Enable: %s\n", sortedEnabledChars);

      return 0;
    }

   //----------------------------------------------------------------
   //Uložení inputu z klávesnice do pole uInput
   //Úprava na velké písmena (case insensitivity)

   strcat(uInput, argv[1]);
   allCaps(uInput);
   lenInput = strlen(uInput);
   
   //-----------------------------------------------------------------
   //Cyklus opakující dokud nenarazí na konec dokumentu (NULL)
   //Zkontroluje zda řádek není delší jak 100 znaků
   //Porovná n-tý znak inputu z klávesnice s n-tým znakem aktuálně vzatého řádku ze souboru
   //Vyhodnotí zda jsou stejné na základě počtu shodných charakterů
   //Pokud ano vloží znak následují po řetězci zadaném uživatelem do řetězce enableChars

   while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
      lenLine = strlen(buffer);
      if (lenLine > 100){
         fprintf(stderr, "ERROR: Line exceeded 100 chars.");
         return 1;
      }
      allCaps(buffer);
      for (int i = 0; i < lenInput; i++){
         if (uInput[i] == buffer[i]){
            matchedChars++;
         }
      }
   // debug log
   // printf("help: %d, %s", matchedChars, buffer);
      if (matchedChars == lenInput){
         if (duplicateCheck(enabledChars, buffer[matchedChars]) == 1){
            enabledChars[enCharIndex] = buffer[matchedChars];
            enCharIndex++;
         }
         matchCount++;
         strcpy(matches, buffer);
      }
      matchedChars = 0;
   }

   // debug log
   // printf("\n match count: %d\n", matchCount);

   //-----------------------------------------------------
   //Vyhodnocení a výpis

   if (matchCount == 1) {
      printf("Found: %s\n",  matches);

   } else if (matchCount == 0) {
      printf("Not found\n");
      
   } else if (matchCount > 1) {
      alphaSort(enabledChars, sortedEnabledChars); 
      if (spaceCheck(sortedEnabledChars)){
         printf("Enable: %s\n", sortedEnabledChars);
         printf("Space is one of the enabled characters.");
         return 0;
      }
      printf("Enable: %s\n", sortedEnabledChars);
   }
   return 0;
}
