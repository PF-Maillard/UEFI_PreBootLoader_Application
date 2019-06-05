#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/BlockIo.h>
#include <Library/DevicePathLib.h>
#include <Protocol/LoadedImage.h>

extern EFI_BOOT_SERVICES *gBS;

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_STATUS Status;
	EFI_HANDLE * HandlesFS, HandleLoad, DeviceHandle;
	EFI_GUID BlockIoGuid = EFI_BLOCK_IO_PROTOCOL_GUID;
	EFI_DEVICE_PATH_PROTOCOL  *DevicePath, *FilePath;
	UINTN i, Nb;

	const CHAR16 *FileName = L"EFI\\Manjaro\\grubx64.efi";

	gBS->LocateHandleBuffer(ByProtocol, &BlockIoGuid, NULL, &Nb,&HandlesFS);
	for(i=0;i<Nb;i++)
	{
			Status = gBS->OpenProtocol(HandlesFS[i], &gEfiDevicePathProtocolGuid, (VOID **)&DevicePath, ImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
			if (!EFI_ERROR(Status))
			{
				gBS->LocateDevicePath(&BlockIoGuid, &DevicePath, &DeviceHandle);
				FilePath = FileDevicePath(HandlesFS[i], FileName);
				gBS->LoadImage(FALSE, ImageHandle, FilePath, NULL, 0, &HandleLoad);
				gBS->StartImage(HandleLoad, NULL, NULL);
		}

	}

	return EFI_SUCCESS;
}




