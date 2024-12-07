#pragma once
#include <array>
#include <cstdint>
#include "ExternalStorage.hpp"
using namespace std;
struct NVFile{
    /**
    *@brief This constructor is to be used only when creating a new file! If the file already exists in the non volatile memory, you are strongly advised to use the second constructor.
    *@param   storage: Indicate the External Storage used to store the data
    *@param File_Adress: It is used to design the adress of the new file, note that this have to be chosen carefully in order to avoid it interfering with another file and to avoid having the data stored outside the memory!
    *@params file_id, total_entries, cell_Size, max_cells, sector_qty, sector_sz, file_type, file_name: As described in the file header format.
    */
	NVFile(ExternalStorage &storage, std::uint32_t File_Adress,  std::uint8_t file_id, std::uint32_t total_entries,  std::uint32_t cell_Size,  std::uint32_t max_cells,  std::uint32_t sector_qty,  std::uint32_t sector_sz,  std::uint8_t file_type,   std::uint8_t *file_name)
	:storage(storage),File_Adress(File_Adress)
	{
	    Header[0]=file_id;
		*(std::int8_t*)(Header+1)=status;
		*(std::int8_t*)(Header+2)=file_status;
		*(std::uint32_t*)(Header+3)=last_entry_cell_id;
		*(std::uint32_t*)(Header+7)=total_entries;
		*(std::uint32_t*)(Header+11)=cell_Size;
		*(std::uint32_t*)(Header+15)=used_cells;
		*(std::uint32_t*)(Header+19)=max_cells;
		*(std::uint32_t*)(Header+23)=sector_qty;
		*(std::uint32_t*)(Header+27)=sector_sz;
		*(std::uint8_t*)(Header+31)=file_type;
		std::copy(file_name,file_name+12,Header+32);
		*(std::uint32_t*)(Header+44)=file_signature;
		Copy_From_Header();
		Clear();
	}
	/**
    *@brief This constructor is to be used only when the file already exists in the non volatile memory! If you want to create a new file, you are strongly advised to use the first constructor.
    *@param   storage: Indicate the External Storage used to store the data
    *@param File_Adress: It is used to design the adress of the new file, note that this have to be chosen carefully in order to avoid it interfering with another file and to avoid having the data stored outside the memory!
    */
	NVFile(ExternalStorage &storage,std::uint32_t File_Adress)
	:storage(storage),File_Adress(File_Adress){
		Read_File_Header();
		Copy_From_Header();
	}
	/**
	*@brief The "Header" is a continuous segment of data containing an exact match to data of the header in the non volatile memory, note that it MUST be sync all the time
	*/
	const static std::uint16_t Header_Size=48;
	std::uint8_t Header[Header_Size];
	ExternalStorage &storage;
	std::uint8_t file_id;
	std::int8_t	status;
	std::int8_t	file_status;
	std::uint32_t last_entry_cell_id;
	std::uint32_t total_entries;
	std::uint32_t cell_Size;
	std::uint32_t used_cells;
	std::uint32_t max_cells;
	std::uint32_t sector_qty;
	std::uint32_t sector_sz;
	std::uint8_t file_type;
	std::uint8_t file_name[12];
	std::uint32_t file_signature;

	const std::uint32_t File_Adress;
	const std::uint32_t Cell_Header_Size=6;
	/**
	*@brief this structure is used to keep track of the number of errors accumulated!
	*/
	struct Errors{
		bool is_Created=false;
		std::uint16_t Writing_Errors=0;
		std::uint16_t Reading_Errors=0;
		std::uint16_t CRC_Errors=0;
	};
	Errors errors;
	/**
	*@brief Updates the variables concerned from the header
	*/
	void Copy_From_Header(){
		file_id=Header[0];
		status=*(std::int8_t*)(Header+1);
		file_status=*(std::int8_t*)(Header+2);
		last_entry_cell_id=*(std::uint32_t*)(Header+3);
		total_entries=*(std::uint32_t*)(Header+7);
		cell_Size=*(std::uint32_t*)(Header+11);
		used_cells=*(std::uint32_t*)(Header+15);
		max_cells=*(std::uint32_t*)(Header+19);
		sector_qty=*(std::uint32_t*)(Header+23);
		sector_sz=*(std::uint32_t*)(Header+27);
		file_type=*(std::uint8_t*)(Header+31);
		std::copy(Header+32,Header+44,file_name);
		file_signature=*(std::uint32_t*)(Header+44);
	}
	/**
	*@brief Sync the header with the description variables
	*@param If set to true, the data will also be stored in the NV memory
	*/
	void Copy_To_Header(bool Save=true){
		Header[0]=file_id;
		*(std::int8_t*)(Header+1)=status;
		*(std::int8_t*)(Header+2)=file_status;
		*(std::uint32_t*)(Header+3)=last_entry_cell_id;
		*(std::uint32_t*)(Header+7)=total_entries;
		*(std::uint32_t*)(Header+11)=cell_Size;
		*(std::uint32_t*)(Header+15)=used_cells;
		*(std::uint32_t*)(Header+19)=max_cells;
		*(std::uint32_t*)(Header+23)=sector_qty;
		*(std::uint32_t*)(Header+27)=sector_sz;
		*(std::uint8_t*)(Header+31)=file_type;
		std::copy(file_name,file_name+12,Header+32);
		*(std::uint32_t*)(Header+44)=file_signature;
		if(Save)Create_File();
	}
	/**
	*@brief Calculate the CRC32 of the segment of data
	*@param pData: A pointer to the start of the data
	*@param dataLen: The length of the data
	*/
	std::uint32_t crc32(std::uint8_t *pData,std::uint16_t dataLen) {
		std::uint32_t crc=0xFFFFFFFF;
		for(size_t i=0;i<dataLen;i++) {
			std::uint8_t Byte=pData[i];
			for(size_t j=0;j<sizeof(std::uint8_t);j++) {
				std::uint32_t b=(Byte^crc)&1;
				crc>>=1;
				if(b) crc=crc^0xEDB88320;
				Byte>>=1;
			}
		}

		return ~crc;
	}
	/**
	*@brief Similar to the Write function in the External Storage, but it updates the count of errors at the same time
	*/
	bool Write(std::uint32_t Adress,std::uint8_t *pData, std::uint32_t dataLen){
		if(!storage.Write(Adress, pData, dataLen)){
			errors.Writing_Errors++;
			return false;
		}
		return true;
	}
	/**
	*@brief Similar to the Read function in the External Storage, but it updates the count of errors at the same time
	*/
	bool Read(std::uint32_t Adress,std::uint8_t *pData, std::uint32_t dataLen){
		if(!storage.Read(Adress, pData, dataLen)){
			errors.Reading_Errors++;
			return false;
		}
		return true;
	}
	/**
	*@brief Verify the CRC of a cell after being copied to the volatile memory
	*@param pData: Points to the starting adress of the copy of the cell
	*/
	bool Verify_CRC(std::uint8_t* pData){
	    std::uint32_t Entry_Size=pData[5]+256*pData[4];
		std::uint32_t crc=crc32(pData+Cell_Header_Size,Entry_Size);
		if(crc!=*(std::uint32_t*)pData){
			errors.CRC_Errors++;
			cout<<"ERROR "<<crc<<" != "<<(*(std::uint32_t*)pData)<<endl;
			return  false;
		}
		return true;
	}
	/**
	*@brief Used to update the signature of a file
	*/
	void Fix_Signature(){}
    /**
    *@brief Loads the header of the file from the NV memory
    */
	bool Read_File_Header(){
		if(! Read(File_Adress, Header, Header_Size))return false;
		Copy_From_Header();
	}
    /**
    *@brief Create a new file in the NV memory
    */
	bool Create_File(){
		return Write(File_Adress, Header, Header_Size);
	}
	/**
	 *@brief it is used to update the content of a cell
	 */
	bool Write_Cell(std::uint32_t Entry_Id,std::uint8_t* pData){
		std::uint32_t Cell_Adress = Get_Cell_Adress(Entry_Id);
		return Write(Cell_Adress, pData, cell_Size);

	}
    /**
	 *@brief it is used to read the content of a cell
	 */
	bool Read_Cell(std::uint32_t Entry_Id,std::uint8_t* pData){
	    if(Entry_Id > used_cells)
            return false;
		std::uint32_t Cell_Adress = Get_Cell_Adress(Entry_Id);
		if(! Read(Cell_Adress, pData, cell_Size)){
			return false;
		}
		return Verify_CRC(pData);
	}
	/**
	*@brief Adding an entry to the end of the file, not the start and the end of the file can vary!
	*/
	bool Add_Entry(std::uint8_t *pData,std::uint16_t dataLen){
		file_status=0;
		std::uint8_t Header[Cell_Header_Size];
		std::uint32_t crc=crc32(pData,dataLen);
		(*(uint32_t*)Header)=crc;
		Header[4]=(dataLen>>8);
		Header[5]=(dataLen&255);
		if ( last_entry_cell_id == max_cells ) last_entry_cell_id = 1;
		else last_entry_cell_id++;
		if(used_cells<max_cells)	used_cells++;
		std::uint32_t Adress=Get_Cell_Adress(used_cells);
		if(! Write(Adress, Header, Cell_Header_Size))return false;
		if(! Write(Adress+6, pData, dataLen))return false;
		std::uint8_t Zeros[cell_Size-dataLen-Cell_Header_Size];
		for(int i=0;i<cell_Size-dataLen-Cell_Header_Size;i++)Zeros[i]=0;
		if(! Write(Adress+6+dataLen, Zeros, cell_Size-dataLen-Cell_Header_Size))return false;
		Copy_To_Header();
		return true;
	}
    /**
	 *@brief it is used to delete an entry
	 */
	bool Delete_Entry(std::uint32_t Entry_Id){
		std::uint8_t Temp[cell_Size];
		for(std::uint32_t i=Entry_Id;i!=last_entry_cell_id;i++){
			if(i > max_cells)i-= max_cells;
			if(!Read_Cell(i+1,Temp))return false;
			if(!Write_Cell(i,Temp))return false;
		}
		last_entry_cell_id--;
		if(last_entry_cell_id==0)last_entry_cell_id=max_cells;
		used_cells--;
		Copy_To_Header();
		return true;
	}
    /**
	 *@brief it is used to read all the content of a file
	 */
	bool Read_File(std::uint8_t *pData){
		for(std::uint32_t i=1;i<=used_cells;i++){
			if(!Read_Cell(i,pData))return false;
			pData+=cell_Size;
		}
		return true;
	}
    /**
	 *@brief it is used to Remove all elements of a file
	 */
	bool Clear(){
		 used_cells=0;
		 last_entry_cell_id=max_cells;
		 file_status=-2;
		 Fix_Signature();
		 Copy_To_Header();
	}
	bool Deep_Clean(){
	    std::uint8_t Zeros[cell_Size];
	    for(int i=0;i<cell_Size;i++)Zeros[i]=0;
        for(int i=0;i<max_cells;i++){
            if(!Write_Cell(i+1,Zeros))return false;
        }
        return Clear();
	}
private:
	std::uint32_t First_Entry_Cell_Id(){
		return (last_entry_cell_id-used_cells+max_cells)%max_cells+1;
	}
	std::uint32_t Get_Cell_Id(std::uint32_t Entry_Id){
		return (First_Entry_Cell_Id()+Entry_Id-2)%max_cells+1;
	}
	std::uint32_t Get_Cell_Adress(std::uint32_t Entry_Id){
		return (Get_Cell_Id(Entry_Id)-1)*cell_Size+File_Adress+Header_Size;
	}
	std::uint32_t Get_Entry_Adress(std::uint32_t Entry_Id){
		return Get_Cell_Adress(Entry_Id)+Cell_Header_Size;
	}

};
