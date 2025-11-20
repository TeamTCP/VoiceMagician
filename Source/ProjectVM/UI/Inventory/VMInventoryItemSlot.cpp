// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMInventoryItemSlot.h"
#include "UI/Inventory/VMInventoryTooltip.h"
#include "UI/Inventory/VMDragItemVisual.h"
#include "Inventory/VMItemDragDropOperation.h"

#include "Item/Equipment/VMEquipmentInfo.h"
#include "Item/Equipment/VMEquipment.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"




void UVMInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (TooltipClass)
	{
		UVMInventoryTooltip* ToolTip = CreateWidget<UVMInventoryTooltip>(this, TooltipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}

	ItemReference = nullptr;
	RefreshFromItem();
}

void UVMInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning,
		TEXT("InventorySlot::NativeConstruct %s Icon=%p Border=%p"),
		*GetName(),
		ItemIcon.Get(),
		ItemBorder.Get());


	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();
	/*if (ItemReference)
	{
		ItemBorder->SetBrushColor(FLinearColor::Gray);
	}

	ItemIcon->SetBrushFromTexture(Info.Icon);

	if (!ItemReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryItemSlot: ItemReference is null in NativeConstruct"));
		return;
	}


	ItemBorder->SetBrushColor(FLinearColor::Gray);

	if (Info.Icon)
	{
		ItemIcon->SetBrushFromTexture(Info.Icon);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryItemSlot: Icon is null for item %s"),
			*Info.ItemName);
	}
	*/

	//if (ItemBorder)
	//{
	//	ItemBorder->SetBrushColor(FLinearColor::Transparent);
	//}

}

FReply UVMInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Slot::OnMouseButtonDown"));

	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply.Unhandled();
}

void UVMInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UVMInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!ItemReference)
		return;

	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

	if (DragItemVisualClass)
	{
		UVMDragItemVisual* DragVisual =
			CreateWidget<UVMDragItemVisual>(this, DragItemVisualClass);

		if (Info.Icon)
		{
			DragVisual->ItemIcon->SetBrushFromTexture(Info.Icon);
		}
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		UVMItemDragDropOperation* DragItemOperation =
			NewObject<UVMItemDragDropOperation>();

		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;
		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;

		UE_LOG(LogTemp, Warning, TEXT("NativeOnDragDetected: drag op created for %s"),
			*Info.ItemName);

	}
}

void UVMInventoryItemSlot::SetItemReference(UVMEquipment* ItemIn)
{ 
	ItemReference = ItemIn;

	UE_LOG(LogTemp, Warning,
		TEXT("InventorySlot(%s in %s)::SetItemReference Item=%s"),
		*GetName(),
		*GetOuter()->GetName(),
		ItemReference ? *ItemReference->GetEquipmentInfo().ItemName : TEXT("NULL"));

	RefreshFromItem();   // 여기서만 처리
};


void UVMInventoryItemSlot::SetUp(const FVMEquipmentInfo& Info)
{

		// 1) 아직 MID가 없다면 한 번만 생성
	if (!ItemMaterialInstance)
	{
		UMaterialInterface* BaseMat = nullptr;

		// Atlas 머터리얼 변수가 따로 있다면 우선 사용
		if (AtlasMaterial)
		{
			BaseMat = AtlasMaterial;
		}
		else if (ItemIcon)
		{
			// 초기 Brush에 물려 있는 M_ItemAtlas (또는 MI_ItemAtlas)를 한 번만 사용
			BaseMat = Cast<UMaterialInterface>(ItemIcon->GetBrush().GetResourceObject());
		}

		if (BaseMat)
		{
			ItemMaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
			ItemIcon->SetBrushFromMaterial(ItemMaterialInstance);

			UE_LOG(LogTemp, Warning,
				TEXT("InventorySlot(%s)::SetUp created MID from %s"),
				*GetName(), *BaseMat->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				TEXT("InventorySlot(%s)::SetUp: BaseMat is NULL"),
				*GetName());
		}
	}

	// 2) 이미 MID가 있으면, 파라미터만 갱신
	if (ItemMaterialInstance)
	{
		ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), Info.AtlasCol);
		ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), Info.AtlasRow);

		UE_LOG(LogTemp, Warning,
			TEXT("InventorySlot(%s)::SetUp set Col=%d Row=%d"),
			*GetName(), Info.AtlasCol, Info.AtlasRow);
	}

}

void UVMInventoryItemSlot::RefreshFromItem()
{

	UE_LOG(LogTemp, Warning,
		TEXT("InventorySlot::RefreshFromItem ENTER, Ref=%s"),
		*GetNameSafe(ItemReference));

	if (SlotType == ESlotType::Inventory)
	{
		ItemBorder->SetBrushColor(FLinearColor(0.4f, 0.0f, 0.4f, 1.0f));  // 보라색
	}
	else if (SlotType == ESlotType::Equipment)
	{
		ItemBorder->SetBrushColor(FLinearColor::White);  // 흰색
	}

	if (!ItemIcon || !ItemBorder)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("RefreshFromItem: ItemIcon or ItemBorder is NULL"));
		return;
	}

	const FLinearColor InventoryColor(0.5f, 0.0f, 0.8f, 1.0f); // 인벤/장비에서 아이템 있는 칸 색 (보라)
	const FLinearColor EquipEmptyColor = FLinearColor::White;  // 장비칸 비었을 때 흰색

	// 1) 빈 슬롯 처리
	if (!ItemReference)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);

		if (SlotType == ESlotType::Inventory)
		{
			// 인벤토리는 아이템 없어도 항상 보라 배경 유지
			ItemBorder->SetBrushColor(InventoryColor);
		}
		else // ESlotType::Equipment
		{
			// 장비칸은 비었으면 흰 배경
			ItemBorder->SetBrushColor(EquipEmptyColor);
		}

		return;
	}

	// 2) 아이템이 있는 경우
	ItemIcon->SetVisibility(ESlateVisibility::Visible);

	// 장비 / 인벤 모두 "아이템 있는 칸"은 보라색으로
	ItemBorder->SetBrushColor(InventoryColor);

	// 2) 베이스 머티리얼에서 MID 생성
	if (!ItemMaterialInstance)
	{
		if (!ItemAtlasBaseMaterial)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("RefreshFromItem: ItemAtlasBaseMaterial is NULL! (set in WBP_VMInventoryItemSlot)"));
			return;
		}

		ItemMaterialInstance = UMaterialInstanceDynamic::Create(ItemAtlasBaseMaterial, this);
		ItemIcon->SetBrushFromMaterial(ItemMaterialInstance);

		UE_LOG(LogTemp, Warning,
			TEXT("RefreshFromItem: MID created=%p"), ItemMaterialInstance.Get());
	}

	if (!ItemMaterialInstance)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("RefreshFromItem: ItemMaterialInstance STILL NULL"));
		return;
	}

	// 3) 아이템의 Atlas 정보 적용
	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();
	ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), Info.AtlasCol);
	ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), Info.AtlasRow);

	UE_LOG(LogTemp, Warning,
		TEXT("RefreshFromItem: Set Atlas Col=%d Row=%d"),
		Info.AtlasCol, Info.AtlasRow);
}




bool UVMInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

FReply UVMInventoryItemSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("InventorySlot::OnMouseButtonDoubleClick"));

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && ItemReference)
	{
		// 더블클릭 시 장착 요청 브로드캐스트
		OnItemDoubleClicked.Broadcast(ItemReference);
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

void UVMInventoryItemSlot::ClearItem()
{
	ItemReference = nullptr;

	if (ItemIcon)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	if (ItemBorder)
	{
		ItemBorder->SetBrushColor(FLinearColor::White);
	}
}