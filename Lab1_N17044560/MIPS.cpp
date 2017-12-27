#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
    public:
        bitset<32> ReadData1, ReadData2; 
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            // implement the funciton by you.
					
			ReadData1 = Registers[RdReg1.to_ulong()]; // reads the value of rs register
			ReadData2 = Registers[RdReg2.to_ulong()]; // reads the value of rt register
			if(WrtEnable.to_ulong() == 1)
			{
				Registers[WrtReg.to_ulong()] = WrtData.to_ulong(); // writes the value to the rd register
			}
		}
		 
	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"\nUnable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   
                 // implement the ALU operations by you. 
				 unsigned int temp_ALUOP=0;
				 unsigned long temp_oprand1=0;
				 unsigned long temp_oprand2=0;
				 unsigned long temp_ALUresult=0;
				 temp_ALUOP = ALUOP.to_ulong();
				 temp_oprand1 = oprand1.to_ulong();
				 temp_oprand2 = oprand2.to_ulong();
				 cout<<"\ntemp_ALUOP is :"<< temp_ALUOP;
				 cout<<"\ntemp_oprand1 is :"<< temp_oprand1;
				 cout<<"\ntemp_oprand2 is :"<< temp_oprand2;
				 
				 switch(temp_ALUOP)
				 {
					 case 1:
					 {
						// ADDU and ADDIU operation //
						temp_ALUresult = (temp_oprand1)+(temp_oprand2);
						break;
					 }
					 
					 case 3:
					 {
						// SUBU operation //
						temp_ALUresult = (temp_oprand1)-(temp_oprand2);
						break;
					 }
					 
					 case 4:
					 {
						// AND operation //
						temp_ALUresult = (temp_oprand1)&(temp_oprand2);
						break;
					 }
					 
					 case 5:
					 {
						// OR operation //
						temp_ALUresult = (temp_oprand1)|(temp_oprand2);
						break;
					 }
					 
					 case 7:
					 {
						// NOR operation //
						temp_ALUresult = ~((temp_oprand1)|(temp_oprand2));
						break;
					 }
					 default:
					 {
					     cout<<"\nInvalid R-Type Instruction";
						 break;
					 }
				 }
				 ALUresult = temp_ALUresult;				 
			 
                 return ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem()
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {      
                        IMem[i] = bitset<8>(line);
                        i++;
                     }
                     
                  }
                  else cout<<"\nUnable to open file";
                  imem.close();
                     
           }
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {    
               // implement by you. (Read the byte at the ReadAddress and the following three byte).
				unsigned long temp_index=0;
				unsigned long temp_instruction=0;
				temp_index = ReadAddress.to_ulong();
					
				   temp_instruction = IMem[temp_index].to_ulong()<<24; // left shift inorder to get the first 8 bits //
				   temp_instruction = (temp_instruction)|(IMem[temp_index+1].to_ulong()<<16); // left shift inorder to get the second 8 bits//
				   temp_instruction = (temp_instruction)|(IMem[temp_index+2].to_ulong()<<8); // left shift inorder to get the third 8 bits//
				   temp_instruction = (temp_instruction)|(IMem[temp_index+3].to_ulong()); // left shift inorder to get the last 8 bits//
				   
				   Instruction = temp_instruction; // 32-bit instruction //
				   return Instruction;
                
              }     

      private:
           vector<bitset<8> > IMem;
};
      

class DataMem    
{
      public:
          bitset<32> readdata;  
          DataMem()
          {
             DMem.resize(MemSize); 
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {      
                        DMem[i] = bitset<8>(line);
                        i++;
                       }
                  }
                  else cout<<"\nUnable to open file";
                  dmem.close();
          
          }  
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
               
               // implement by you.
			   
			   unsigned long readindex=0;
			   unsigned long writeindex=0;
			   bitset<8> temp_readdata[4];
			   bitset<8> byte1 = 0, byte2 = 0, byte3 = 0, byte4 = 0;
			   string stringtemp_readdata = "", stringtemp_writedata = "", temp_byte1 = "", temp_byte2 = "",temp_byte3 = "",temp_byte4 = "";
			   
			   // check if a value is to be written to data memory //
			   if(writemem.to_ulong() == 1)
			   {
				writeindex = Address.to_ulong();
				stringtemp_writedata = WriteData.to_string();
				temp_byte1 = stringtemp_writedata.substr(0,8);
				temp_byte2 = stringtemp_writedata.substr(8,8);
				temp_byte3 = stringtemp_writedata.substr(16,8);
				temp_byte4 = stringtemp_writedata.substr(24,8);
				
				byte1 = bitset<8>(temp_byte1);
				byte2 = bitset<8>(temp_byte2);
				byte3 = bitset<8>(temp_byte3);
				byte4 = bitset<8>(temp_byte4);
								
				DMem[writeindex] = byte1;
				DMem[writeindex+1] = byte2;
				DMem[writeindex+2] = byte3;
				DMem[writeindex+3] = byte4;
				
			   }
				// check if a value is to read from data memory //
				
			else if(readmem.to_ulong() == 1)
			{
				
				for(int i=0;i<=3;i++)
					   {		   
					   readindex = Address.to_ulong()+i;
					   temp_readdata[i] = DMem[readindex];
					   }
					   stringtemp_readdata = temp_readdata[0].to_string()+temp_readdata[1].to_string()+temp_readdata[2].to_string()+temp_readdata[3].to_string();
					   readdata = bitset<32> (stringtemp_readdata);
				
			}
			else
			{
					// do nothing //
			
			}
			
			return readdata;     
		  }
			
				                     
          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {     
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"\nUnable to open file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};

class Decoder
{

	public:
		bitset<1> decoderWriteEnable;
		bitset<1> decoderReadMem;
		bitset<1> decoderWriteMem;
		bitset<1> DecoderLogic(bitset<32> decoderinstruction)
		{
			unsigned long temp_decoderWriteEnable=0;
			unsigned long temp_decoderReadMem=0;
			unsigned long temp_decoderWriteMem=0;
			unsigned long decoderopcode=0;
			decoderopcode = decoderinstruction.to_ulong()>>26;
			switch(decoderopcode)
			{
				case 0x00:
				{
					// Control signals for ALU operation //
					temp_decoderWriteEnable = 1;
					temp_decoderReadMem = 0;
					temp_decoderWriteMem = 0;
					break;
				}
				case 0x23:
				{
					// Control signals for Load word operation //
					temp_decoderWriteEnable = 1;
					temp_decoderReadMem = 1;
					temp_decoderWriteMem = 0;
					break;
				}
				case 0x2B:
				{
					// Control signals for Store Word operation //
					temp_decoderWriteEnable = 0;
					temp_decoderReadMem = 0;
					temp_decoderWriteMem = 1;
					break;
				}
				case 0x09:
				{
					// Control signals for ADDIU operation //
					temp_decoderWriteEnable = 1;
					temp_decoderReadMem = 0;
					temp_decoderWriteMem = 0;
					break;
				}
				default:
				{
				    cout<<"\nInvalid opcode";
				    break;
				}
				
			}
			decoderWriteEnable = temp_decoderWriteEnable;
			decoderReadMem = temp_decoderReadMem;
			decoderWriteMem = temp_decoderWriteMem;
			
			return decoderWriteEnable;
			return decoderReadMem;
			return decoderWriteMem;
		}
};
void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
{
                  ofstream fileout;
                  
                  fileout.open("Results.txt",std::ios_base::app);
                  if (fileout.is_open())
                  {
                  
                  fileout <<pc<<' '<<WrRFAdd<<' '<<WrRFData<<' '<<RFWrEn<<' '<<WrMemAdd<<' '<<WrMemData<<' '<<WrMemEn << endl;
                     
                  }
                  else cout<<"\nUnable to open file";
                  fileout.close();

}

   
int main()
{   
    bitset<32> pc=0;
	RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
	Decoder myDecoder;
		
	
	// userdefined //
	bitset<6> opcode=0;
	bitset<3> aluopcode=0;
	bitset<32> aluoperand1=0;
	bitset<32> aluoperand2=0;
	bitset<5> readregister1=0;
	bitset<5> readregister2=0;
	bitset<5> writeregister=0;
	bitset<32> immregister=0;
	bitset<1> immbit=0;
	bitset<32> temp_immregister=0;
	bitset<32> immregister1=0;
	bitset<32> temp_result=0;
	bitset<5> WriteRFAddress=0;
	bitset<32> WriteRFData=0;
	bitset<1> RFWriteEnable=0;
	bitset<32> WriteMemoryAddress=0;
	bitset<32> WriteMemoryData=0;
	bitset<1> WriteMemoryEnable=0;
	bitset<32> tempPC=0;
	bitset<32> temp1=0;
	bitset<32> temp2=0;
	bitset<32> jAddress=0;
	bitset<32> currentPC=0;
	bitset<32> beqAddress=0;
	bool beq_flag=0;
	bool beq1_flag=0;
	bool temp_aluex = 0;
	bool temp_lwex = 0;
	bool temp_swex = 0;
	bool temp_beqex = 0;
	bool temp_addiex = 0;
	bool temp_halt=0;
	bool temp_jex = 0;
	bool j_flag=0;
	
   
    while (1) //each loop body corresponds to one clock cycle.
	{
		// MEM and WB stage //
		
			if(temp_aluex == 1)
			{
				// MEM stage //
				WriteMemoryAddress = 0;
				WriteMemoryData=0;	
				WriteMemoryEnable = 0;
					
				// WB stage //
				myRF.ReadWrite(bitset<5> (0),bitset<5> (0),writeregister.to_ulong(),myALU.ALUresult.to_ulong(),myDecoder.decoderWriteEnable.to_ulong());
					
				temp_aluex = 0;
				
			}			
			else if(temp_lwex == 1)
			{
				// MEM stage //
				myDataMem.MemoryAccess(immregister1.to_ulong(),bitset<32> (0),myDecoder.decoderReadMem.to_ulong(),myDecoder.decoderWriteMem.to_ulong());
				WriteMemoryAddress = 0;
				WriteMemoryData = 0;
				WriteMemoryEnable = 0;
								
				cout<<"\nThe data read from data memory is :"<< myDataMem.readdata.to_ulong();
				
				// WB stage //
				myRF.ReadWrite(bitset<5> (0), bitset<5> (0), writeregister.to_ulong(), myDataMem.readdata.to_ulong(), myDecoder.decoderWriteEnable.to_ulong());
				
				temp_lwex = 0;
			}
			else if(temp_swex == 1)
			{
				// MEM stage //
								
				myDataMem.MemoryAccess(immregister1.to_ulong(),myRF.ReadData2.to_ulong(),myDecoder.decoderReadMem.to_ulong(),myDecoder.decoderWriteMem.to_ulong());
				WriteMemoryAddress = immregister1.to_ulong();
				WriteMemoryData = myRF.ReadData2.to_ulong();
				WriteMemoryEnable = myDecoder.decoderWriteMem.to_ulong();
					
				// WB stage //
				
				temp_swex = 0;			
			}
			else if(temp_beqex == 1)
			{
				// MEM stage //
				WriteMemoryAddress = 0;
				WriteMemoryData=0;
				WriteMemoryEnable = 0;
				
				// WB stage //
						
				temp_beqex = 0;
				
			}
			else if(temp_addiex == 1)
			{
				// MEM stage //
				WriteMemoryAddress = 0;
				WriteMemoryData=0;
				WriteMemoryEnable = myDecoder.decoderWriteMem.to_ulong();
								
				// WB stage //
				myRF.ReadWrite(bitset<5> (0),bitset<5> (0),readregister2.to_ulong(),myALU.ALUresult.to_ulong(),myDecoder.decoderWriteEnable.to_ulong());
							
				temp_addiex = 0;
			}
			else if(temp_jex == 1)			
			{
				// MEM stage //
				WriteMemoryAddress = 0;
				WriteMemoryData=0;
				WriteMemoryEnable = 0;

				// WB stage //
							
				temp_jex = 0;
			}
			else
			{
				// nothing //
				
			}
				
		// ID/RF and EX stage //
		
		
		if(myInsMem.Instruction.to_ulong() != 0xFFFFFFFF)
		{
		cout<<"\nThe value of instruction is :"<<myInsMem.Instruction.to_ulong();
		opcode = myInsMem.Instruction.to_ulong()>>26;
		
		switch(opcode.to_ulong())
		{
			case 0x00:
			{
				// ID/RF Stage //
				
				if(beq_flag == 1)
				{
					beq1_flag = 1;
				}
				else
				{
				
				readregister1 = ((myInsMem.Instruction.to_ulong()) & (0x03E00000))>>21;
				readregister2 = ((myInsMem.Instruction.to_ulong()) & (0x001F0000))>>16;
				writeregister = ((myInsMem.Instruction.to_ulong()) & (0x0000F800))>>11;
				
				myRF.ReadWrite(readregister1.to_ulong(),readregister2.to_ulong(),bitset<5> (0),bitset<32> (0),bitset<1> (0));
				cout<<"\nThe values in the register1 :"<< myRF.ReadData1.to_ulong();
				cout<<"\nThe values in the register2 :"<< myRF.ReadData2.to_ulong();
				myDecoder.DecoderLogic(myInsMem.Instruction.to_ulong());
								
				// EX stage //
				
				aluopcode = ((myInsMem.Instruction.to_ulong()) & 0x00000007);	
				aluoperand1 = myRF.ReadData1.to_ulong();
				aluoperand2 = myRF.ReadData2.to_ulong();
				myALU.ALUOperation(aluopcode,aluoperand1,aluoperand2);
				cout<<"\nThe ALU output is : "<<myALU.ALUresult.to_ulong();
				
				
				WriteRFAddress = writeregister.to_ulong();
				RFWriteEnable = myDecoder.decoderWriteEnable.to_ulong();
				WriteRFData = myALU.ALUresult.to_ulong();
				
				temp_aluex = 1;
				}
				break;
			}
			case 0x09:
			{
				// ID/RF Stage //
				
				if(beq_flag == 1)
				{
					beq1_flag = 1;
				}
				else
				{
				
				readregister1 = ((myInsMem.Instruction.to_ulong()) & (0x03E00000))>>21;
				readregister2 = ((myInsMem.Instruction.to_ulong()) & (0x001F0000))>>16;
				writeregister = ((myInsMem.Instruction.to_ulong()) & (0x03E00000))>>21;
				
				myRF.ReadWrite(readregister1.to_ulong(),readregister2.to_ulong(),bitset<5> (0),bitset<32> (0),bitset<1> (0));
				cout<<"\nThe values in the register1 :"<< myRF.ReadData1.to_ulong();
				cout<<"\nThe values in the register2 :"<< myRF.ReadData2.to_ulong();
				myDecoder.DecoderLogic(myInsMem.Instruction.to_ulong());
				immregister = ((myInsMem.Instruction.to_ulong()) & (0x0000FFFF));
				immbit = ((immregister.to_ulong()) & (0x8000))>>15;
				if(immbit.to_ulong() != 1)
				{
					temp_immregister = ((immregister.to_ulong()) | (0x00000000));
				}
				else 
				{
					temp_immregister = ((immregister.to_ulong()) | (0xFFFF0000));
				}
				immregister1 = temp_immregister.to_ulong();
				
				// EX stage //
				
				aluopcode = ((myInsMem.Instruction.to_ulong()) & 0x1C000000)>>26;	
				aluoperand1 = myRF.ReadData1.to_ulong();
				aluoperand2 = immregister1.to_ulong();
				myALU.ALUOperation(aluopcode,aluoperand1,aluoperand2);
				cout<<"\nThe ALU output is : "<<myALU.ALUresult.to_ulong();
				RFWriteEnable = myDecoder.decoderWriteEnable.to_ulong();
				WriteRFData = myALU.ALUresult.to_ulong();
				WriteRFAddress = writeregister.to_ulong();
				temp_addiex = 1;
				}
				break;
			}
			
			case 0x23:
			{
				// ID/RF Stage //
				
				if(beq_flag == 1)
				{
					beq1_flag = 1;
				}
				else
				{
					readregister1 = ((myInsMem.Instruction.to_ulong()) & (0x03E00000))>>21;
					readregister2 = ((myInsMem.Instruction.to_ulong()) & (0x001F0000))>>16;
					writeregister = ((myInsMem.Instruction.to_ulong()) & (0x001F0000))>>16;
					
					myRF.ReadWrite(readregister1, readregister2, bitset<5> (0), bitset<32> (0), bitset<1> (0));
					cout<<"\nThe values in the register1 :"<< myRF.ReadData1.to_ulong();
					cout<<"\nThe values in the register2 :"<< myRF.ReadData2.to_ulong();
					immregister = ((myInsMem.Instruction.to_ulong()) & (0x0000FFFF));
					immbit = ((immregister.to_ulong()) & (0x8000))>>15;
					if(immbit.to_ulong() != 1)
					{
						temp_immregister = ((immregister.to_ulong()) | (0x00000000));
					}
					else 
					{
						temp_immregister = ((immregister.to_ulong()) | (0xFFFF0000));
					}
					immregister1 = ((myRF.ReadData1.to_ulong()) | (temp_immregister.to_ulong()));
					cout<<"\nThe address in Data Memory from which the value should be read is :" << immregister1.to_ulong();
					myDecoder.DecoderLogic(myInsMem.Instruction.to_ulong());
					
					RFWriteEnable = myDecoder.decoderWriteEnable.to_ulong();
					WriteRFData = myDataMem.readdata.to_ulong();
					WriteRFAddress = writeregister.to_ulong();
					
					temp_lwex = 1;
				}
				break;
			}
			
			case 0x2B:
			{
				// ID/RF stage //
				
				if(beq_flag == 1)
				{
					beq1_flag = 1;
				}
				else
				{
				readregister1 = ((myInsMem.Instruction.to_ulong()) & (0x03E00000))>>21;
				readregister2 = ((myInsMem.Instruction.to_ulong()) & (0x001F0000))>>16;
				myRF.ReadWrite(readregister1, readregister2, bitset<5> (0), bitset<32> (0), bitset<1> (0));
				cout<<"\nThe values in the register1 :"<< myRF.ReadData1.to_ulong();
				cout<<"\nThe values in the register2 :"<< myRF.ReadData2.to_ulong();
				immregister = ((myInsMem.Instruction.to_ulong()) & (0x0000FFFF));
				immbit = ((immregister.to_ulong()) & (0x8000))>>15;
				if(immbit.to_ulong() != 1)
				{
					temp_immregister = ((immregister.to_ulong()) | (0x00000000));
				}
				else 
				{
					temp_immregister = ((immregister.to_ulong()) | (0xFFFF0000));
				}
				
				// EX stage // 
				
				immregister1 = ((myRF.ReadData1.to_ulong()) | (temp_immregister.to_ulong()));
				cout<<"\nThe address in Data Memory to which the value should be written is :" << immregister1.to_ulong();
				myDecoder.DecoderLogic(myInsMem.Instruction.to_ulong());
				RFWriteEnable = myDecoder.decoderWriteEnable.to_ulong();
				WriteRFData = 0;
				WriteRFAddress = 0;
				
				temp_swex = 1;
				}
				break;
				
			}

			case 0x04:
			{
				// ID/RF stage //
				readregister1 = ((myInsMem.Instruction.to_ulong()) & (0x03E00000))>>21;
				readregister2 = ((myInsMem.Instruction.to_ulong()) & (0x001F0000))>>16;
				
				myRF.ReadWrite(readregister1, readregister2, bitset<5> (0), bitset<32> (0), bitset<1> (0));
				cout<<"\nThe values in the register1d :"<< myRF.ReadData1.to_ulong();
				cout<<"\nThe values in the register2 :"<< myRF.ReadData2.to_ulong();
				immregister = ((myInsMem.Instruction.to_ulong()) & (0x0000FFFF));
				if(myRF.ReadData1.to_ulong() == myRF.ReadData2.to_ulong())
				{
					beq_flag = 1;				
					immbit = ((immregister.to_ulong()) & (0x8000))>>15;
					if(immbit.to_ulong() != 1)
					{
						temp_immregister = ((immregister.to_ulong()) | (0x00000000))<<2;
										
					}
					else 
					{
						temp_immregister = ((immregister.to_ulong()) | (0x3FFF0000))<<2;
					}
					beqAddress = temp_immregister.to_ulong();
					
				}
				cout<< "\nbranch address is :" <<beqAddress.to_ulong();
				WriteRFAddress = 0;
				RFWriteEnable = 0;
				WriteRFData = 0;
				
				
				temp_beqex = 1;
				break;
			}
			case 0x02:
			{
				immregister = ((myInsMem.Instruction.to_ulong()) & (0x03FFFFFF));
				tempPC = pc.to_ulong()>>28;
				temp1 = immregister.to_ulong()<<2;
				temp2 = tempPC.to_ulong()<<28;
				jAddress = temp2.to_ulong()|temp1.to_ulong();
				cout<<"\nThe value of jump target address is :"<<jAddress.to_ulong();
				RFWriteEnable = 0;
				WriteRFData = 0;
				WriteRFAddress = 0;	
				
				j_flag = 1;
				temp_jex = 1;
				break;
			}
			
			case 0xFF:
			{
				cout<<"\nInvalid opcode";
				break;
			}
			default:
			{
				// nothing //
			}
		}
		}
		else
		{
			temp_halt = 1;
		}
		
		// IF stage
		
		if(temp_halt == 1)
		{
			break;
		}
		else
		{	
			//Checks if the flag for branch instruction is set //
			if(beq1_flag == 1)
			{
				pc = pc.to_ulong()+beqAddress.to_ulong()-4;
				currentPC = pc.to_ulong();
				myInsMem.ReadMemory(pc);
				pc = pc.to_ulong()+4;
				cout<<"\nThe Instruction read from Instruction Memory is :" << myInsMem.Instruction.to_ulong();
				cout<<"\nCurrent PC :"<<currentPC.to_ulong();
				beq_flag = 0;
				beq1_flag = 0;
			}
			//Checks if the flag for jump instruction is set //
			else if(j_flag == 1)
			{
				myInsMem.ReadMemory(jAddress.to_ulong());
				cout<<"\nThe Instruction read from Instruction Memory is :" << myInsMem.Instruction.to_ulong();
				currentPC = jAddress.to_ulong();
				pc = jAddress.to_ulong()+4;
				cout<<"\nCurrent PC :"<<currentPC.to_ulong();
				j_flag = 0;
			}
			else
			{
				myInsMem.ReadMemory(pc);
				cout<<"\nThe Instruction read from Instruction Memory is :" << myInsMem.Instruction.to_ulong();
				currentPC = pc.to_ulong();
				cout<<"\nCurrent PC :"<<currentPC.to_ulong();
				pc = pc.to_ulong()+4;
			
			}
		}
		
			
    // At the end of each cycle, fill in the corresponding data into "dumpResults" function to output files.
    // The first dumped pc value should be 0.	
		if(currentPC.to_ulong() == 0)
		{
			dumpResults(currentPC,0,0,0,0,0,0);      
		}
		else
		{
			dumpResults(currentPC,WriteRFAddress,WriteRFData,RFWriteEnable,WriteMemoryAddress,WriteMemoryData,WriteMemoryEnable);
		}
	}
	      myRF.OutputRF(); // dump RF; 
          myDataMem.OutputDataMem(); // dump data mem
      
          return 0;
}