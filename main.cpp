#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include "transport_udp.h"

#include <vector>

int main()
{
	std::cout<<"--------------- Opencv Image Transport ---------------"<<std::endl;
	//-------------- Read A Image ---------------//
	cv::Mat src = cv::imread("dog.jpg");
	cv::Mat src2;
	cv::resize( src, src2, cv::Size( 400, 400 ) );
	cv::imshow("test", src2);
	
	//-------------- Init a Socket Client-------------//	
	pcs::Transport *udp = new pcs::TransportUDP();
	udp->initSocketClient();
	
	//-------------- Encode ------------//
	std::vector<unsigned char> data_encode;
	std::vector<int> quality;
	quality.push_back( CV_IMWRITE_JPEG_QUALITY );
	quality.push_back( 50 );
	cv::imencode( ".jpg", src2, data_encode, quality );

	std::cout<<"size of the encode data: "<<data_encode.size()<<std::endl;
	char encodeArray[65535];
		
	int size = data_encode.size();
	for( size_t i = 0; i < size; ++ i ){
		encodeArray[i] = data_encode[i];
	}

	struct sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr("192.168.8.240");
	clientAddr.sin_port = htons(2333);
	
	int fd = udp->getClientFd();
	std::cout<<"udp fd = "<<fd<<std::endl;
	int ret = udp->write(fd, (unsigned char *)encodeArray, size, clientAddr);
	std::cout<<"send "<<ret<<" bytes data"<<std::endl;	

	cv::waitKey(0);

	/*char *data = "hello World";
	while(1){
		struct sockaddr_in clientAddr;
	        clientAddr.sin_family = AF_INET;
	        clientAddr.sin_addr.s_addr = inet_addr("192.168.8.240");
        	clientAddr.sin_port = htons(2333);
        
	        int fd = udp->getClientFd();
        	std::cout<<"udp fd = "<<fd<<std::endl;
	        int ret = udp->write(fd, (unsigned char *)data, 11, clientAddr);
        	std::cout<<"send "<<ret<<" bytes data"<<std::endl;

		sleep(2);
	}*/
	
	return 0;
}
