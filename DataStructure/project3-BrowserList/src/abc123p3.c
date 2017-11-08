#include <string.h>
#include <stdio.h>
#include "BrowserList.h"
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
{
    int iDelimPos;                      // found position of delim
    int iCopy;                          // number of characters to copy
    char szDelims[20] = " \n\r";        // delimiters. The first character is
    szToken[0] = '\0';                  // an space.

    // Check for no token if at zero byte
    if (*pszInputTxt == '\0')
        return NULL;

    // get the position of the first delim
    iDelimPos = strcspn(pszInputTxt, szDelims);

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > iTokenSize)
        iCopy = iTokenSize;             // truncated size
    else
        iCopy = iDelimPos;

    // copy the token into the target token variable
    memcpy(szToken, pszInputTxt, iCopy);
    szToken[iCopy] = '\0';              // null terminate
}


int main()
{
    Element element;
    BrowserList browserList = newBrowserList();
    char szRecordType[10];
    while(fgets(element.szURL, MAX_URL_LENGTH, stdin) != NULL){
      if(element.szURL[0]=='\n')
        continue;
      
      getToken(element.szURL,szRecordType,sizeof(szRecordType)-1);

      if(0==strcmp(szRecordType, "PRINT")){
        printBrowserList(browserList); 
      }else if(0==strcmp(szRecordType, "BACK")){
        back(browserList); 
      }else if(0==strcmp(szRecordType, "FORWARD")){
        forward(browserList); 
      }else{
        goToURL(browserList, element);
      }
    }
    //freeBrowserList(browserList);
    return 0;
}
