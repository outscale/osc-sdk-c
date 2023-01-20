/* C++ compatible example */

#include "osc_sdk.h"
#include "json/json.h"
#include <iostream>
#include <memory>

int main(void)
{
	const std::unique_ptr<osc_env, decltype(&osc_destroy_sdk)>
		sdk(osc_new_sdk(NULL, 0), &osc_destroy_sdk);
	const std::unique_ptr<osc_str, decltype(&osc_destroy_str)>
		ret_str(osc_new_str(), &osc_destroy_str);
	osc_read_images_arg arg_filter = {0};
	Json::Value root;
	char alias[] = "Outscale";
	char *account_aliases[] = {alias, NULL};
	Json::CharReaderBuilder builder;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	JSONCPP_STRING err;

	arg_filter.filters.account_aliases = account_aliases;
	arg_filter.is_set_filters = 1;

	if (osc_read_images(&(*sdk), &*ret_str, &arg_filter) < 0)
		return 1;

	if (!reader->parse(ret_str->buf, ret_str->buf + ret_str->len, &root,
			   &err)) {
		std::cout << "error" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << root << std::endl;
	return 0;
}
