// Diego La Rosa 2022

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ElimAnnouncement.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UElimAnnouncement : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetElimAnnouncementText(FString AttakerName, FString VictimName);

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* AnnouncementBox;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AnnouncementText;

	float l;
};
