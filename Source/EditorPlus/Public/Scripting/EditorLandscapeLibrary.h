#pragma once

#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetData.h"

#include "EditorLandscapeLibrary.generated.h"

class ALandscape;
class ALandscapeProxy;

UCLASS()
class EDITORPLUS_API UEditorLandscapeLibrary 
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Landscape")
    static void ImportHeightmap(ALandscapeProxy* Landscape, const FString& FilePath = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Landscape")
	static void ImportWeightmap(ALandscapeProxy* Landscape, const FString& LayerName, const FString& FilePath = TEXT(""));

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Landscape")
	static void ReimportMaps(ALandscapeProxy* Landscape);

    UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Landscape")
    static ALandscape* ResizeLandscape(ALandscapeProxy* Landscape, const int32 ComponentCount, const int32 SectionsPerComponent, const int32 QuadsPerSection);

	UFUNCTION(BlueprintCallable, Category = "Editor Scripting | Landscape")
	static void SetLocationZ(ALandscapeProxy* Landscape, const float LocationZ);

private:
	static bool ReadHeightmapFile(TArray<uint16>& Result, const FString& Filename, int32 ExpectedWidth, int32 ExpectedHeight);
	static bool ReadWeightmapFile(TArray<uint8>& Result, const FString& Filename, FName LayerName, int32 ExpectedWidth, int32 ExpectedHeight);
	static void SetLevelPosition(const FIntVector& Position);
};
