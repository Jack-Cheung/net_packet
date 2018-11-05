#include "client.hpp"
#include "../util/include/packet.hpp"
#include <boost/program_options.hpp>
#include <boost/typeof/typeof.hpp>
#include <thread>
#include <fstream>
using namespace boost::program_options;
int main(int argc, char const *argv[])
{


    const string PARAM_ADDRESS = "address";
    const string PARAM_PUBKEY = "publickey";
    const string PARAM_CAPACITY = "capcity";
    const string PARAM_AVAIL = "available";
    const string PARAM_LOCATION = "location";
    const string PARAM_SIG = "signature";
    const string PARAM_FILELIST = "filelist";
    const string PARAM_EXAMSERIAL = "examserial";
    const string PARAM_EXAMLENGTH = "examlength";
    const string PARAM_EXAM = "exam";
    const string PARAM_FILECNT = "filecount";
    const string PARAM_FILEID = "fileid";
    const string PARAM_CNT = "count";
    const string PARAM_ANSER = "anwser";
    const string PARAM_FILESIZE = "filesize";
    const string PARAM_BLOCKCNT = "blockcount";
    const string PARAM_BLOCKNUM = "blocknum";
    const string PARAM_BLOCKSIZE = "blocksize";
    const string PARAM_BLOCK = "block";
    const string PARAM_VERIFYID = "verifyId";

 
/*
("fileid,i",  value<string>()->required()->default_value("file id 256bit"),"")
    ("verifyId,c",value<string>()->required()->default_value("encrypted sha256 "),"")
    ("available,a", value<uint8_t>()->required()->default_value(1),"")
    ("signature,s",value<string>()->required()->default_value("d3754d4604e54c0b70a8




*/
    


    options_description opts_all("storage node command line");
    positional_options_description pod;
    pod.add("cmd", 1);
    opts_all.add_options()
    ("cmd", "register, report etc")
    ("help,h", "help info")
    ("address,A", value<string>()->required()->default_value("test wallet url"), "wallet url")
    ("publickey,k", value<string>()->required()->default_value("ff3f4036a1164d1ddbad5b3edf9022addb3e1961a54a922708a6c1ffc49e5489"), "public key for wallet")
    ("capcity,c", value<uint64_t>()->required()->default_value(80), "capcity of this storage node")
    ("available,a",value<uint64_t>()->required()->default_value(20),"available space of this storage node")
    ("location,l",value<uint64_t>()->required()->default_value(1),"location of this storage node")
    ("signature,s",value<string>()->required()->default_value("d3754d4604e54c0b70a84eb38a52b3d2b5c7f7d1af44e4bc7b5478a6f4061edd"),"location of this storage node")
    ("filelist,f",value<string>()->required(),"file id list, unfixed size = n * 256");

    options_description opt_register("register");
    opt_register.add_options()
    ("cmd", "register, report etc")
    ("help,h", "help info")
    ("address,A", value<string>()->required()->default_value("test wallet url"), "wallet url")
    ("publickey,k", value<string>()->required()->default_value("ff3f4036a1164d1ddbad5b3edf9022addb3e1961a54a922708a6c1ffc49e5489"), "public key for wallet")
    ("capcity,c", value<uint64_t>()->required()->default_value(80), "capcity of this storage node")
    ("available,a",value<uint64_t>()->required()->default_value(20),"available space of this storage node")
    ("location,l",value<uint64_t>()->required()->default_value(1),"location of this storage node")
    ("signature,s",value<string>()->required()->default_value("d3754d4604e54c0b70a84eb38a52b3d2b5c7f7d1af44e4bc7b5478a6f4061edd"),"location of this storage node")
    ;
    options_description opt_report("report");
    opt_report.add_options()
    ("cmd", "register, report etc")
    ("help,h", "help info")
    ("capcity,c",  value<uint64_t>()->required()->default_value(80),"capcity of this storage node")
    ("available,a",value<uint64_t>()->required()->default_value(20),"available space of this storage node")
    ("location,l", value<uint64_t>()->required()->default_value(1),"location of this storage node")
    ("signature,s",value<string>()->required()->default_value("d3754d4604e54c0b70a84eb38a52b3d2b5c7f7d1af44e4bc7b5478a6f4061edd"),"location of this storage node")
    ("filelist,f",value<string>()->required()->default_value(string(64, 'x')), "file id list, unfixed size = n * 256")
    ;
    options_description opt_exam("exam");
    opt_exam.add_options()
    ("cmd", "register, report etc")
    ("help,h", "help info")
    ("examserial,s",  value<uint64_t>()->required()->default_value(80),"")
    ("examlength,L",value<uint32_t>()->required()->default_value(20),"")
    ("exam, e", value<uint64_t>()->required()->default_value(1),"?")
    ("signature,s",value<string>()->required()->default_value("d3754d4604e54c0b70a84eb38a52b3d2b5c7f7d1af44e4bc7b5478a6f4061edd"),"")
    ("filecount,c",value<uint16_t>()->required()->default_value(100), "")
    ("fileid,i",value<uint64_t>()->required()->default_value(1), "")
    ("count,C",value<uint8_t>()->required()->default_value(50), "")
    ("anwser,a",value<string>()->required()->default_value("mine is right anwser"), "")
    ;

    options_description opt_send("sendfile");
    opt_send.add_options()
    ("cmd", "register, report etc")
    ("help,h", "help info")
    ("filesize,s",  value<uint64_t>()->required()->default_value(80),"")
    ("blockcount,c",value<uint8_t>()->required()->default_value(20),"")
    ("blocknum, n", value<uint8_t>()->required()->default_value(1),"")
    ("blocksize, b", value<uint8_t>()->required()->default_value(1),"?")
    ("block, B", value<string>()->required()->default_value(string(512, 'B')), " file block data")
    ("signature,s",value<string>()->required()->default_value("d3754d4604e54c0b70a84eb38a52b3d2b5c7f7d1af44e4bc7b5478a6f4061edd"),"")
    ;

    options_description opt_save("savefile");
    opt_save.add_options()
    ("cmd", "register, report etc")
    ("help,h", "help info")
    ("fileid,i",  value<string>()->required()->default_value("file id 256bit"),"")
    ("verifyId,c",value<string>()->required()->default_value("encrypted sha256 "),"")
    ("available,a", value<uint8_t>()->required()->default_value(1),"")
    ("signature,s",value<string>()->required()->default_value("d3754d4604e54c0b70a84eb38a52b3d2b5c7f7d1af44e4bc7b5478a6f4061edd"),"")
    ;

    variables_map vm;
    
    try
    {
        BOOST_AUTO(pr ,command_line_parser(argc, argv).options(opts_all).positional(pod).run());
        store(pr, vm);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }
    if(vm.count("cmd"))
    {
        string cmd = vm["cmd"].as<string>();
        Header header;
        Packet pkt;
        if (cmd == "register") 
        {
            variables_map _vm;
            store(command_line_parser(argc, argv).options(opt_register).positional(pod).run(), _vm);
            if (_vm.count("help")) 
            {
                cout << opt_register << endl;
            }
            else
            {
                string address = _vm["address"].as<string>();
                string publickey = _vm["publickey"].as<string>();
                uint64_t capcity = _vm["capcity"].as<uint64_t>();
                uint64_t available = _vm[PARAM_AVAIL].as<uint64_t>();
                uint64_t location = _vm[PARAM_LOCATION].as<uint64_t>();
                string signature = _vm[PARAM_SIG].as<string>();
                client cl;
                cl.Register(address, publickey, capcity, available, location, signature);
                cl.run();
            }
        }
        else if(cmd == "report")
        {
            variables_map _vm;
            store(command_line_parser(argc, argv).options(opt_report).positional(pod).run(), _vm);
            uint64_t capcity = _vm[PARAM_CAPACITY].as<uint64_t>();
            uint64_t available = _vm[PARAM_AVAIL].as<uint64_t>();
            uint64_t location = _vm[PARAM_LOCATION].as<uint64_t>();
            string signature = _vm[PARAM_SIG].as<string>();
            string filelist = _vm[PARAM_FILELIST].as<string>();
            client cl;
            cl.Report(capcity, available, location, signature, filelist);
            cl.run(false);
        }
        else if(cmd == "exam")
        {
            variables_map _vm;
            store(command_line_parser(argc, argv).options(opt_exam).positional(pod).run(), _vm);
            uint64_t examserial = _vm[PARAM_EXAMSERIAL].as<uint64_t>();
            uint32_t examlength = _vm[PARAM_EXAMLENGTH].as<uint32_t>();
            string signature = _vm[PARAM_SIG].as<string>();
            uint16_t filecount =  _vm[PARAM_FILECNT].as<uint16_t>();
            uint64_t fileid =  _vm[PARAM_FILEID].as<uint64_t>();
            uint8_t count = _vm[PARAM_CNT].as<uint8_t>();
            string anwser = _vm[PARAM_ANSER].as<string>();
            client cl;
            cl.Exam(examserial, examlength, signature, filecount, fileid, count, anwser);
            cl.run(false);
        }
        else if(cmd == "sendfile")
        {
            variables_map _vm;
            store(command_line_parser(argc, argv).options(opt_send).positional(pod).run(), _vm);
            uint64_t filesize = _vm[PARAM_FILESIZE].as<uint64_t>();
            uint8_t blockcount = _vm[PARAM_BLOCKCNT].as<uint8_t>();
            uint8_t blocknum = _vm[PARAM_BLOCKNUM].as<uint8_t>();
            uint8_t blocksize = _vm[PARAM_BLOCKSIZE].as<uint8_t>();
            string block = _vm[PARAM_BLOCK].as<string>();
            string signature = _vm[PARAM_SIG].as<string>();
            client cl;
            cl.SendFile(filesize, blockcount, blocknum, blocksize, block, signature);
            cl.run(false);
        }
        else if(cmd == "savefile")
        {
            variables_map _vm;
            store(command_line_parser(argc, argv).options(opt_save).positional(pod).run(), _vm);
            string fileid = _vm[PARAM_FILEID].as<string>();
            string verifyId = _vm[PARAM_VERIFYID].as<string>();
            uint8_t avail = _vm[PARAM_AVAIL].as<uint8_t>();
            string sig = _vm[PARAM_SIG].as<string>();
            client cl;
            cl.SaveFile(fileid, verifyId, avail, sig);
            cl.run(false);
        }
        else
        {
            cout << "unknown command : " << cmd << "\n command: register/report\n";
        }
    }
    else
    {
        cout << "command: register/report\n";
    }


    return 0;
}