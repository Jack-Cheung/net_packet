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
    ("capcity,c", "capcity of this storage node")
    ("available,a","available space of this storage node")
    ("location,l","location of this storage node")
    ("filelist,f","file id list, unfixed size = n * 256")
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
                //cout << "  ----" << address  << publickey << capcity;
                /* ofstream os;
                os.open("test1.txt", fstream::out | fstream::binary);
                os << pkt;
                os.close();
                os.clear();
                ifstream is;
                is.open("test1.txt", fstream::in | fstream::binary);
                Packet p;
                is >> p;
                os.open("test3.txt", fstream::out);
                os << p;
                os.close();
                std::cout << "client start." << sizeof(long) << std::endl; */
                client cl;
                cl.Register(address, publickey, capcity, available, location, signature);
                cl.run();
                
            }
        }
        else if(cmd == "report")
        {

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
/*     try
    {
        std::cout << "client start." << sizeof(long) << std::endl;
        client cl;
        cl.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
     
    return 0; */
}