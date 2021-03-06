#include "EditorStaticMeshLibrary2.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "MeshDescription.h"

TArray<FString> UEditorStaticMeshLibrary2::GetMaterialSlots(UStaticMesh* Object)
{
    check(Object);

    TArray<FString> Result;
    for (auto& Material : Object->StaticMaterials)
        Result.Add(Material.MaterialSlotName.ToString());
    
    return Result;
}

void UEditorStaticMeshLibrary2::SetMaterialSlot_ByName(UStaticMesh* Object, const FString& Name, UMaterialInterface* Material)
{
    check(Object);

    FName Key = *Name;
    auto Index = Object->StaticMaterials.IndexOfByPredicate([Key](const FStaticMaterial& Mtl) { return Mtl.MaterialSlotName == Key; });
    if (Index == INDEX_NONE)
        return;

    SetMaterialSlot_ByIndex(Object, Index, Material);
}

void UEditorStaticMeshLibrary2::SetMaterialSlot_ByIndex(UStaticMesh* Object, const int32 Index, UMaterialInterface* Material)
{
    check(Object);

    Object->SetMaterial(Index, Material);
}

void UEditorStaticMeshLibrary2::SetMeshBuildSettings(UStaticMesh* Object, bool bRecomputeTangents, bool bRecomputeNormals)
{
    check(Object);

    auto NumLODs = Object->GetNumLODs();
    for (auto i = 0; i < NumLODs; i++)
    {
        FStaticMeshSourceModel& SourceModel = Object->SourceModels[i];
        SourceModel.BuildSettings.bRecomputeNormals = bRecomputeNormals;
        SourceModel.BuildSettings.bRecomputeTangents = bRecomputeTangents;
    }

    Object->Build(false);
    Object->PostEditChange();
    Object->MarkPackageDirty();
}

void UEditorStaticMeshLibrary2::SetUVConstant(UStaticMesh* Object, const uint8& Channel, const FVector2D& UV)
{
    check(Object);

    auto LODCount = Object->GetNumLODs();
    for (auto LODIndex = 0; LODIndex < LODCount; LODIndex++)
    {
        if (Object->GetNumUVChannels(LODIndex) - 1 != Channel)
        {
            auto bChannelWasAdded = Object->InsertUVChannel(LODIndex, Channel);
            if (!bChannelWasAdded)
            {
                while (Object->GetNumUVChannels(LODIndex) - 1 < Channel)
                    bChannelWasAdded = Object->AddUVChannel(LODIndex);
            }
        }

        auto MeshDescription = Object->GetOriginalMeshDescription(LODIndex);

        TArray<FVector2D> TexCoords;
        TexCoords.Init(UV, MeshDescription->VertexInstances().Num());

        Object->SetUVChannel(LODIndex, Channel, TexCoords);
    }
}