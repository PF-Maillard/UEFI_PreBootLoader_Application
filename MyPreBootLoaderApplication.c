#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/BlockIo.h>
#include <Library/DevicePathLib.h>
#include <Protocol/LoadedImage.h>

//  
//  Simplification of the EFI_BOOT_SERVICES structure by an identifier
//
extern EFI_BOOT_SERVICES *gBS;

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_STATUS Status;
	EFI_HANDLE * HandlesFS, HandleLoad, DeviceHandle;
	EFI_GUID BlockIoGuid = EFI_BLOCK_IO_PROTOCOL_GUID;
	EFI_DEVICE_PATH_PROTOCOL  *DevicePath, *FilePath;
	UINTN i, Nb;

	//  
	//  Character string for the GRUB path (Manjaro)
	//
	const CHAR16 *FileName = L"EFI\\Manjaro\\grubx64.efi";

    //  
    //  We locate our list of BlockIo handles that represent the partitions
    //
	Status = gBS->LocateHandleBuffer(ByProtocol, &BlockIoGuid, NULL, &Nb,&HandlesFS);
	if (EFI_ERROR(Status))
	{
		Print(L"ERROR (main): Impossible de recupérer la liste des handles du protocole BlockIO\n");
		return Status;
	}
	for(i=0;i<Nb;i++)
	{
		//  
		//  If possible, we get it back from the path
		//
		Status = gBS->OpenProtocol(HandlesFS[i], &gEfiDevicePathProtocolGuid, (VOID **)&DevicePath, ImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
		if (!EFI_ERROR(Status))
		{
			//  
			//  We'll test the path on each of the partitions
			//
			Status = gBS->LocateDevicePath(&BlockIoGuid, &DevicePath, &DeviceHandle);
			if (EFI_ERROR(Status))
			{
				Print(L"ERROR (main): Impossible de recuperer le DevicePath\n");
				return Status;
			}
			
			FilePath = FileDevicePath(HandlesFS[i], FileName);
			if (EFI_ERROR(Status))
			{
				Print(L"ERROR (main): Impossible de recupérer le chemin\n");
				return Status;
			}
			
			//  
			//  We load our object into memory and start it
			//
			Status = gBS->LoadImage(FALSE, ImageHandle, FilePath, NULL, 0, &HandleLoad);
			if (EFI_ERROR(Status))
			{
				Print(L"ERROR (main): Impossible de charger l'image en memoire\n");
				return Status;
			}
			
			Status = gBS->StartImage(HandleLoad, NULL, NULL);
			if (EFI_ERROR(Status))
			{
				Print(L"ERROR (main): Impossible de demarrer l'image en memoire\n");
				return Status;
			}
		}

	}

	return EFI_SUCCESS;
}




