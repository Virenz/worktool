#include "vtapi.h"

VtApi::VtApi()
{
	chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */
}

VtApi::~VtApi()
{
	if (chunk.memory)
	{
		free(chunk.memory);
		chunk.memory = NULL;
	}
	chunk.size = 0;
}

size_t WriteData(void* ptr, size_t size, size_t nmemb, void* stream)
{
	size_t realsize = size * nmemb;
	_MEMORYSTRUNCT* mem = (_MEMORYSTRUNCT*)stream;

	mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), ptr, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

bool VtApi::VtScanFile(const char * filepath)
{
	bool isSuccess = false;

	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	static const char header_buf[] = "Expect:";
	long http_response_code = 0;
	CHAR *data = (CHAR*)malloc(600);
	memset(data, 0, 600);

	curl_global_init(CURL_GLOBAL_ALL);

	/* Fill in the file upload field */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "file",
		CURLFORM_FILE, filepath,
		CURLFORM_END);

	/* Fill in the filename field */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "filename",
		CURLFORM_COPYCONTENTS, filepath,
		CURLFORM_END);

	/* Fill in the submit field too, even if this is rarely needed */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "apikey",
		CURLFORM_COPYCONTENTS, "63efab1f4d9bd879a836a9a38917d89265aedb261ea334df9637fef108512821",
		CURLFORM_END);

	curl = curl_easy_init();
	/* initalize custom header list (stating that Expect: 100-continue is not
	wanted */
	headerlist = curl_slist_append(headerlist, header_buf);
	if (curl) {
		/* what URL that receives this POST */

		curl_easy_setopt(curl, CURLOPT_URL, "https://www.virustotal.com/vtapi/v2/file/scan");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, header_buf);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		//通过write_data方法将联网返回数据写入到data中
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			isSuccess = false;
		}
		else
		{
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
			if (http_response_code == 200)
			{
				isSuccess = true;
				//MessageBox(hDlg, StringToWchar_t(data), L"Example", MB_OK | MB_ICONINFORMATION);
			}
		}
		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all(headerlist);
	}
	return isSuccess;
}

bool VtApi::VtRescanFile(const char * sourses)
{
	bool isSuccess = false;

	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	static const char header_buf[] = "Expect:";
	long http_response_code = 0;
	CHAR *data = (CHAR*)malloc(600);
	memset(data, 0, 600);

	curl_global_init(CURL_GLOBAL_ALL);

	/* Fill in the file  sha1/md5/sha256 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "resource",
		CURLFORM_COPYCONTENTS, sourses,
		CURLFORM_END);

	/* Fill in the apikey */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "apikey",
		CURLFORM_COPYCONTENTS, "63efab1f4d9bd879a836a9a38917d89265aedb261ea334df9637fef108512821",
		CURLFORM_END);

	curl = curl_easy_init();
	/* initalize custom header list (stating that Expect: 100-continue is not
	wanted */
	headerlist = curl_slist_append(headerlist, header_buf);
	if (curl) {
		/* what URL that receives this POST */

		curl_easy_setopt(curl, CURLOPT_URL, "https://www.virustotal.com/vtapi/v2/file/rescan");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, header_buf);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		//通过write_data方法将联网返回数据写入到data中
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			isSuccess = false;
		}
		else
		{
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
			if (http_response_code == 200)
			{
				isSuccess = true;
				//MessageBox(hDlg, StringToWchar_t(data), L"Example", MB_OK | MB_ICONINFORMATION);
			}
		}
		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all(headerlist);
	}
	return isSuccess;
}

bool VtApi::VtReport(const char * sourses)
{
	bool isSuccess = false;
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	static const char header_buf[] = "Expect:";
	long http_response_code = 0;
	CHAR *data = (CHAR*)malloc(20480);
	memset(data, 0, 20480);

	curl_global_init(CURL_GLOBAL_ALL);

	/* Fill in the file  sha1/md5/sha256 */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "resource",
		CURLFORM_COPYCONTENTS, sourses,
		CURLFORM_END);

	/* Fill in the apikey */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "apikey",
		CURLFORM_COPYCONTENTS, "63efab1f4d9bd879a836a9a38917d89265aedb261ea334df9637fef108512821",
		CURLFORM_END);

	curl = curl_easy_init();
	/* initalize custom header list (stating that Expect: 100-continue is not
	wanted */
	headerlist = curl_slist_append(headerlist, header_buf);
	if (curl) {
		/* what URL that receives this POST */

		curl_easy_setopt(curl, CURLOPT_URL, "https://www.virustotal.com/vtapi/v2/file/report");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, header_buf);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		//通过write_data方法将联网返回数据写入到data中

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		/* Perform the request, res will get the return code */
		
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			isSuccess = false;
		}
		else
		{
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
			if (http_response_code == 200)
			{
				printf("%s memory retrieved\n", chunk.memory);
				isSuccess = true;
			}
		}

		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all(headerlist);
	}
	return  isSuccess;
}

char* VtApi::getReportJson()
{
	return this->chunk.memory;
}

void VtApi::cleanChunk()
{
	free(chunk.memory);
	chunk.memory = NULL;
	chunk.size = 0;
}
