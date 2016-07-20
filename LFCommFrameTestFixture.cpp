#include "gtest/gtest.h"

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <ios>
#include <iomanip>
#include <utility>
#include <string> 
#include <ostream>
#include <iostream>

#include <QtCore>

#include <boost/io/ios_state.hpp>

#include <log4cxx/logger.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>

#include <LFCommCommandEvent.h>

#ifndef ERROR
#define ERROR	-1
#endif
#ifndef OK
#define OK      0
#endif

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::helpers;

int                        g_masterFileDescriptor = -1;  

LoggerPtr rootLogger = nullptr;
LoggerPtr mainLogger = nullptr;

void Print8AsHex(std::ostream &os, uint8_t byte)
{
    boost::io::ios_flags_saver  ifs(os);
    os << std::endl;
    os << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (0u + byte);
    os << std::endl;
}

void Print16AsHex(std::ostream &os, uint16_t byte)
{
    boost::io::ios_flags_saver  ifs(os);

    os << std::endl;
    os << std::setfill('0') << std::setw(4) << std::hex << std::uppercase << (0u + byte);
    os << std::endl;
}

class LFCommFrameTestFixture : public testing::Test 
{ 
public: 
    LFCommFrameTestFixture() 
    : mp_LFCommCommandFrame(nullptr)
    { 
		rootLogger = Logger::getRootLogger();
		mainLogger = Logger::getLogger("parul");  
		std::string configFilename = "/home/lfuser/work/lfcommframe_test/ParulTestConfig.xml";

		struct stat fileStats;
		if (OK == stat(configFilename.c_str(), &fileStats))
		{
			if (fileStats.st_mode & S_IRUSR)
			{
				DOMConfigurator::configure(configFilename.c_str());
				LOG4CXX_INFO(mainLogger, "Log4cxx configured successfully with configuration file \"" << configFilename.c_str() << "\". ");
			}
			else
			{
				BasicConfigurator::configure();
				LOG4CXX_WARN(mainLogger, "Log configuration file \"" << configFilename.c_str() << "\" has insufficient permissions. Errno :-> " << strerror(errno));
				LOG4CXX_INFO(mainLogger, "Log4cxx is going to assume BasicConfigurator default configurations.");            
			}
		}
		else
		{
			BasicConfigurator::configure();
			LOG4CXX_WARN(mainLogger, "Log configuration file \"" << configFilename.c_str() << "\" cannot be stat'ed. Errno :-> " << strerror(errno));
			LOG4CXX_INFO(mainLogger, "Log4cxx is going to assume BasicConfigurator default configurations.");
		}
		
		LOG4CXX_INFO(mainLogger, "============================================================");
		LOG4CXX_INFO(mainLogger, "Starting LFCommFrameTestFixture - built: " << __DATE__ << ", " << __TIME__);
		LOG4CXX_INFO(mainLogger, "============================================================");
    } 
    ~LFCommFrameTestFixture()  
    { 
        // cleanup any pending stuff, but no exceptions allowed
        std::cout << "LFCommFrameTestFixture: Exiting application." << std::endl;
    }
    
protected:
    void SetUp() 
    { 
        // code here will execute just before each test ensues 
        mp_LFCommCommandFrame = new std::LFCommCommandEvent();
    }

    void TearDown() 
    { 
        std::cout << "LFCommFrameTestFixture: Cleaning up ..." << std::endl;
        
        // code here will be called just after each test completes
        // ok to throw exceptions from here if need be
        delete mp_LFCommCommandFrame;
        mp_LFCommCommandFrame = nullptr;
    }
    
    // This the object that is actually Under Test.
     std::LFCommCommandEvent*      mp_LFCommCommandFrame;
    //LFCommResponseEvent *          mp_LFCommResponseFrame;;
};
	
TEST_F(LFCommFrameTestFixture, testLFCommFrameCreation_addChar)
{
    // Begin test case proper...
    QByteArray theInputData;
    
    // MsgTMInitLFCommMode
    // LFComm Frame Length :-> 13	LFComm Frame Data :-> 0xF1, 1A, 08, F0, BC, 00, 00, 03, 04, 00, 00, 59, F2
    theInputData.resize(13);
    theInputData[0] = 0xF1;
    theInputData[1] = 0x1A;
    theInputData[2] = 0x08;
    theInputData[3] = 0xF0;
    theInputData[4] = 0xBC;
    theInputData[5] = 0x07;
    theInputData[6] = 0x06;
    theInputData[7] = 0x03;
    theInputData[8] = 0x04;
    theInputData[9] = 0x00;
    theInputData[10] = 0x00;
    theInputData[11] = 0x59;
    theInputData[12] = 0xF2;
    
    // use loop to addChar
    for (int i = 0; i < theInputData.size(); ++i) 
    {
        mp_LFCommCommandFrame->addChar((unsigned char)theInputData.at(i));
    }
    std::cout << "mp_LFCommCommandFrame -> " << *mp_LFCommCommandFrame << std::endl;
  /*  
    // Debug prints...
    Print16AsHex(std::cout, mp_LFCommCommandFrame->getLFCommand());
    Print8AsHex(std::cout, mp_LFCommCommandFrame->getSenderId());
    Print8AsHex(std::cout, mp_LFCommCommandFrame->getTargetId());
    Print8AsHex(std::cout, mp_LFCommCommandFrame->getFrameByteCount());

    // Parul, pay attention to how I accomplished the EXPECT_EQ verification on the hex value outputs.    
    EXPECT_EQ(true, (0x0000 == mp_LFCommCommandFrame->getLFCommand())) << "Error! LFCommCommandEvent::getLFCommand() verification failed!!";
    EXPECT_EQ(true, (0xF0 == mp_LFCommCommandFrame->getTargetId())) << "Error! LFCommCommandEvent::getTargetId()() verification failed!!";   
    EXPECT_EQ(true, (0xBC == mp_LFCommCommandFrame->getSenderId())) << "Error! LFCommCommandEvent::getSenderId()() verification failed!!";   
    //...
    

    mp_LFCommCommandFrame->clear() ;
    EXPECT_EQ(mp_LFCommCommandFrame->isEmpty() , true);
    
    std::cout << "LFCommFrameTestFixture: We detected the terminate here ..." << std::endl;  */
} 

TEST_F(LFCommFrameTestFixture, testLFCommFrameCreation_addArray)
{
    // Begin test case proper...
    QByteArray theInputData;
    
    // MsgTMGetVersionInfo
    // LFComm Frame Length :-> 11	LFComm Frame Data :-> 0xF1, 1A, 06, F0, BC, 00, 02, 01, 09, 5A, F2
    theInputData.resize(11);
    theInputData[0] = 0xF1;
    theInputData[1] = 0x1A;
    theInputData[2] = 0x06;
    theInputData[3] = 0xF0;
    theInputData[4] = 0xBC;
    theInputData[5] = 0x00;
    theInputData[6] = 0x02;
    theInputData[7] = 0x01;
    theInputData[8] = 0x09;
    theInputData[9] = 0x5A;
    theInputData[10] = 0xF2;


    //std::cout << "LFCommFrameTestFixture: We detected the terminate here ..." << theInputData[0] << std::endl;
    Print16AsHex(std::cout, theInputData[0]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[1]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[2]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[3]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[4]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[5]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[6]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[7]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[8]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[9]);
    std::cout << std::endl;
    Print16AsHex(std::cout, theInputData[10]);
    std::cout << std::endl;

    std::cout << theInputData.size() << std::endl;

   //textEdit->setText(QString(theInputData.toHex())); 

   // test addArray
    for (int i = 0; i < theInputData.size(); ++i) 
    {
        mp_LFCommCommandFrame->addChar((unsigned char)theInputData.at(i));
    }
    std::cout << "mp_LFCommCommandFrame -> " << *mp_LFCommCommandFrame << std::endl;
    
    /*mp_LFCommCommandFrame->addArray((unsigned char *)theInputData.data(), theInputData.size());
    std::cout << "mp_LFCommCommandFrame -> " << *mp_LFCommCommandFrame << std::endl; */


    
    // Debug prints...
    Print16AsHex(std::cout, mp_LFCommCommandFrame->getLFCommand());
    Print8AsHex(std::cout, mp_LFCommCommandFrame->getSenderId());
    Print8AsHex(std::cout, mp_LFCommCommandFrame->getTargetId());
    Print8AsHex(std::cout, mp_LFCommCommandFrame->getFrameByteCount());
   
   
    EXPECT_EQ(true, (0xBC00 == mp_LFCommCommandFrame->getLFCommand())) << "Error! LFCommCommandEvent::getLFCommand() verification failed!!";
    EXPECT_EQ(true, (0xF3 == mp_LFCommCommandFrame->getTargetId())) << "Error! LFCommCommandEvent::getTargetId()() verification failed!!";  
    EXPECT_EQ(true, (0xF0 == mp_LFCommCommandFrame->getSenderId())) << "Error! LFCommCommandEvent::getSenderId()() verification failed!!";    
    //...
    
    
    mp_LFCommCommandFrame->clear() ;
    EXPECT_EQ(mp_LFCommCommandFrame->isEmpty() , true);
    
    std::cout << "LFCommFrameTestFixture: We detected the terminate here ..." << std::endl;  
}    
/*
TEST_F(LFCommFrameTestFixture, testLFCommFrameReceive_receiveDataDecode)
{
    // Begin test case proper...
    // Use a different message here.
    
    // Try to create the MsgTMGetVersionInfo frame using the interface function receiveData/DecodeData:
    // Begin test case proper...
    QByteArray theInputData;
    theInputData.resize(5);
    theInputData[0] = 0xF1;
    theInputData[1] = 0x1A;
    theInputData[2] = 0x64;
    theInputData[3] = 0x18;
    theInputData[4] = 0xca;
    // Pay attention to byte-stuffing...
    theInputData.size();
    theInputData.constData();

    char *ptr = (char*)malloc( sizeof(unsigned char) * 24 );
    memset( ptr, 0, 24 ) ;
    strncpy( ptr, "\xF1\x1A\x06\xF0\xBC\x00\x02\x01\x09\x5A\xF2", 23);
    
   //unsigned char myData[12] = {0xF1,0x1A,0x06,0xF0,0xBC,0x00,0x02,0x01,0x09,0x5A,0xF2, 0};
   //unsigned char * tempData = &myData[0];

   EXPECT_EQ(true, mp_LFCommCommandFrame->receiveData(reinterpret_cast<unsigned char *>(ptr), 23));
       std::cout << "mp_LFCommCommandFrame -> " << *mp_LFCommCommandFrame << std::endl;
   mp_LFCommCommandFrame->decodeReceivedData();
    std::cout << "mp_LFCommCommandFrame -> " << *mp_LFCommCommandFrame << std::endl;
    			
	// Assert this line to ensure the values you set above was okay.
	//EXPECT_EQ(true, mp_currentLFCommCommandFrame->isValid());
    
    // Do more gets here and asserts to validate the contents of the frame.
    EXPECT_EQ(mp_LFCommCommandFrame->getLFCommand(), 0x0002);
    EXPECT_EQ(mp_LFCommCommandFrame->getSenderId(), 0xBC); 
       
    //std::cout << "size " << sizeof(myData) << std::endl;
    std::cout << "LFCommFrameTestFixture: We detected the terminate here ..." << std::endl;
} */

int main(int argc, char **argv) 
{   		
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS(); 
}

