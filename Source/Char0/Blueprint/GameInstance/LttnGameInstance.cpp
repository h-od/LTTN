#include "LttnGameInstance.h"

#include "Char0/Blueprint/Data/Save/LttnSave.h"
#include "Kismet/GameplayStatics.h"


void ULttnGameInstance::Save()
{
	if (ULttnSave* SaveGameInstance = Cast<ULttnSave>(
		UGameplayStatics::CreateSaveGameObject(ULttnSave::StaticClass())))
	{
		SaveGameInstance->Lights = Lights;
		SaveGameInstance->Body = Body;
		SaveGameInstance->Joints = Joints;
			
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, SaveSlotIndex))
		{
			//TODO Feedback?
			UE_LOG(LogTemp, Warning, TEXT("ULttnGameInstance::SaveGame Successfully"));
		}
		else
		{
			// TODO send error to the UI or solve? Why would this ever fail? No free space or something?
			UE_LOG(LogTemp, Warning, TEXT("ULttnGameInstance::SaveGame FAILED"));
		}
	}
}

void ULttnGameInstance::OnStart()
{
	Super::OnStart();
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveSlotIndex))
	{
		if (const ULttnSave* LoadedGame = Cast<ULttnSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveSlotIndex)))
		{
			Lights = LoadedGame->Lights;
			Body = LoadedGame->Body;
			Joints = LoadedGame->Joints;
		}
		else
		{
			//TODO panic!
		}
	}
	else
	{
		Save();
	}
}

void ULttnGameInstance::SetLightsColour(const FLinearColor NewValue)
{
	Lights = NewValue;
	Save();
}

void ULttnGameInstance::SetBodyColour(const FLinearColor NewValue)
{
	Body = NewValue;
	Save();
}

void ULttnGameInstance::SetJointsColour(const FLinearColor NewValue)
{
	Joints = NewValue;
	Save();
}
