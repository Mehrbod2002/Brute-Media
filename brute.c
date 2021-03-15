#include <curl/curl.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#define URL "i.instagram.com/api/v1/accounts/login/ig_sig_key_version=4&signed_body="
#define igsig "4f8732eb9ba7d1c8e8897a75d6474d4eb3f5279137431b2aafb71fafe2abe178"
#define igv 4
struct url_data {
  size_t size;
  char *data;
  char *header;
};
size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;
    data->size += (size * nmemb);
    tmp = realloc(data->data, data->size + 1);
    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }
    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';
    return size * nmemb;
}
char *handle_url(char *url){
 CURL *curl;
 struct url_data data;
 data.size=0;
 data.data=malloc(4096);
 if(NULL==data.data){
  printf("failed malloc\n");
  return NULL;
 }
 data.data[0]='\0';
 CURLcode res;
 curl = curl_easy_init();
 if(curl){
  curl_easy_setopt(curl,CURLOPT_URL,url);
  curl_easy_setopt(curl,CURLOPT_POST,0);
  curl_easy_setopt(curl,CURLOPT_WRITEDATA,&data);
  curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION,data.header);
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
  res = curl_easy_perform(curl);
  if(res != CURLE_OK){
   printf("Not good : %s\n",curl_easy_strerror(res));
  }
  curl_easy_cleanup(curl);
 }
 return data.data;
}
int main(int argc,char* argv[]){
 char phone_id[100];
 char username[50];
 char password[50];
 char* url = "https://www.instagram.com/accounts/login/";
 char* result = handle_url(url);
 char* html = strstr(result,"csrf_token");
 char* token = strtok(html,"\":\"");
 while(token!=NULL){
   if(strcmp(token,"csrf_token")==0){
     token = strtok(NULL,"\":\"");
     break;
   }
   token = strtok(NULL,"\":\"");
 }
 free(result);
 strcpy(phone_id,"\"{'phone_id':'32abb45c-f605-4fd7-9b53-674115516b80','_csrftoken':'");
 strcat(phone_id,token);
 strcat(phone_id,"','username':'");
 strcpy(username,"Username");
 strcat(phone_id,username);
 strcat(phone_id,"','guid':'67ca220c-a9eb-4240-b173-2d253808904d','device_id':'android-283ance46cb0a0bcef4','password':'");
 strcpy(password,"Password");
 strcat(phone_id,password);
 strcat(phone_id,"','login_attempt_count':'0'}\"");
 const unsigned char *en = (const unsigned char *)phone_id;
 char json_data[] = "";
 unsigned char *results = NULL;
 unsigned int resultlen = -1;
 unsigned char *buf= HMAC(EVP_sha256(),igsig,strlen(igsig),en,strlen((char *)en),results,&resultlen);
 for (unsigned char i=0;i<resultlen;i++){
  sprintf(json_data,"%02hhX",buf[i]);
 }
 char *url_encoded = curl_escape(phone_id,strlen(phone_id));
 strcat(URL,json_data);
 strcat(URL,".");
 strcat(URL,url_encoded);
 printf("\nURL %s",URL);
 return 0;
}
