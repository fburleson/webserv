#include "VHost.hpp"

void	VHost::_process_get_method(const t_httprequest &request, t_httpresponse &response) const
{
	if (!file_exists(this->_parse_resource(request.resource)))
	{
		response.status = HTTP_NOT_FOUND;
		response.body = this->_get_err_page(HTTP_NOT_FOUND);
	}
	else
	{
		response.status = HTTP_OK;
		response.body = ftobyte(this->_parse_resource(request.resource));
	}
	response.head.insert({"Content-Length", std::to_string(response.body.size())});
	response.head.insert({"Content-Type", "text/html"});
}

