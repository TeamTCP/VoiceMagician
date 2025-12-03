// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMInventoryItemSlot.h"
#include "UI/Inventory/VMInventoryTooltip.h"
#include "UI/Inventory/VMDragItemVisual.h"
#include "Inventory/VMItemDragDropOperation.h"

#include "Item/Equipment/VMEquipmentInfo.h"
#include "Item/Equipment/VMEquipment.h"

#include "UI/Inventory/VMInventoryPanel.h"      
#include "UI/Inventory/VMEquipmentPanel.h"     
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"




void UVMInventoryItemSlot::NativeOnInitialized()
{
	
	UE_LOG(LogTemp, Warning,
		TEXT("Slot::NativeOnInitialized - %s"), *GetName());

	Super::NativeOnInitialized();

}

void UVMInventoryItemSlot::NativeConstruct()
{
	if (bEnableTooltip && TooltipClass)
	{
		// 가능하면 OwningPlayer 사용하는 게 좋음
		UVMInventoryTooltip* ToolTip =
			CreateWidget<UVMInventoryTooltip>(GetOwningPlayer(), TooltipClass);

		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);

		UE_LOG(LogTemp, Warning,
			TEXT("Slot::NativeConstruct - Tooltip SET OK"));
	}
	else
	{
		SetToolTip(nullptr);
		UE_LOG(LogTemp, Warning,
			TEXT("Slot::NativeConstruct - Tooltip DISABLED"));
	}


	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

}

FReply UVMInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Slot::MouseButtonDown - %s"), *GetName());

		// UMG 권장 패턴: DetectDragIfPressed -> NativeReply
		return UWidgetBlueprintLibrary::DetectDragIfPressed(
			InMouseEvent,
			this,                   // ★ 드래그 타겟은 무조건 this
			EKeys::LeftMouseButton
		).NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UVMInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UVMInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UE_LOG(LogTemp, Warning, TEXT("Slot::NativeOnDragDetected - %s"),
		*GetName());

	if (!ItemReference)   // 슬롯이 들고 있는 장비 포인터
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeOnDragDetected: ItemReference is NULL"));
		return;
	}

	// 1) DragDropOperation 생성
	UVMItemDragDropOperation* DragOp =
		Cast<UVMItemDragDropOperation>(
			UWidgetBlueprintLibrary::CreateDragDropOperation(
				UVMItemDragDropOperation::StaticClass()));

	if (!DragOp)
	{
		UE_LOG(LogTemp, Warning, TEXT("NativeOnDragDetected: DragOp is NULL"));
		return;
	}

	// 2) ★ 여기서 반드시 Payload 를 채워준다
	DragOp->Payload = ItemReference;       // UVMEquipment* 를 넘김
	DragOp->DefaultDragVisual = this;      // 드래그 시 보여줄 위젯
	DragOp->Pivot = EDragPivot::MouseDown;

	OutOperation = DragOp;

	/*UE_LOG(LogTemp, Warning,
		TEXT("NativeOnDragDetected: drag op created for %s (Op=%p, Payload=%p)"),
		*ItemReference->GetName(), DragOp, DragOp->Payload);*/
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

	OnItemDoubleClicked.Clear();

	if (SlotType == ESlotType::Equipment)
	{
		// 장비 패널 이벤트로 바인딩
		if (EquipmentPanelRef)
		{
			OnItemDoubleClicked.AddDynamic(
				EquipmentPanelRef,
				&UVMEquipmentPanel::HandleEquipmentSlotDoubleClicked);
		}
	}
	else
	{
		// 인벤토리 패널 이벤트로 바인딩
		if (InventoryPanelRef)
		{
			OnItemDoubleClicked.AddDynamic(
				InventoryPanelRef,
				&UVMInventoryPanel::HandleItemDoubleClicked);
		}
	}
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
			const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();
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
	UVMItemDragDropOperation* DragOp = Cast<UVMItemDragDropOperation>(InOperation);
	if (!DragOp || !DragOp->SourceItem)
		return false;

	// 슬롯 위 드롭 → 슬롯 이동 처리
	if (IsHovered())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("OnDrop: Dropped ON SLOT %s"), *GetName());

		// TODO: 인벤토리/장비 간 이동 로직
		return true;
	}

	// 슬롯 영역 좌표 계산
	FVector2D ScreenDropPosition = InDragDropEvent.GetScreenSpacePosition();
	const FSlateLayoutTransform& Transform = InGeometry.GetAccumulatedLayoutTransform();
	FSlateRect SlateRect = InGeometry.GetLayoutBoundingRect();

	FVector2D TopLeft = TransformPoint(Transform, SlateRect.GetTopLeft());
	FVector2D BottomRight = TransformPoint(Transform, SlateRect.GetBottomRight());

	bool bIsDropInside =
		ScreenDropPosition.X >= TopLeft.X &&
		ScreenDropPosition.X <= BottomRight.X &&
		ScreenDropPosition.Y >= TopLeft.Y &&
		ScreenDropPosition.Y <= BottomRight.Y;

	// UI 바깥에서 드롭 → 월드에 아이템 떨어뜨리기
	if (!bIsDropInside)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("OnDrop: Dropped OUTSIDE UI → Dropping to WORLD"));

		DragOp->SourceInventory->DropItemToWorld(
			DragOp->SourceItem);

		return true;
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UVMInventoryItemSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	UVMItemDragDropOperation* DragOp = Cast<UVMItemDragDropOperation>(InOperation);
	if (!DragOp || !DragOp->SourceItem || !DragOp->SourceInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("DragCancelled: invalid DragOp"));
		return;
	}

	UE_LOG(LogTemp, Warning,
		TEXT("DragCancelled: dropping '%s' to world"),
		*DragOp->SourceItem->GetName());

	// 여기서 월드 드랍
	DragOp->SourceInventory->DropItemToWorld(
		DragOp->SourceItem);
}

FReply UVMInventoryItemSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning,
		TEXT("Slot::OnMouseButtonDoubleClick - THIS=%s, SlotType=%d, Vis=%d"),
		*GetNameSafe(this),
		(int32)SlotType,
		(int32)GetVisibility());

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && ItemReference)
	{
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