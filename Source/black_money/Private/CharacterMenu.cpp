// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMenu.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "black_moneyGameInstance.h" 
#include "EventCenter.h"

void UCharacterMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // 获取玩家角色引用
    PlayerCharacter = Cast<Ablack_moneyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // 检查进度条是否绑定成功
    if (!HealthBar)
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthBar not bound in CharacterMenu!"));
    }
}

void UCharacterMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 每帧更新血量显示
    UpdateHealthDisplay();
    UpdateManaDisplay();
    UpdateCooldownDisplay();  // 技能冷却
    UpdateHpPotionCountDisplay();
    UpdateMpPotionCountDisplay();
}

void UCharacterMenu::UpdateHealthDisplay()
{
    if (!HealthBar || !PlayerCharacter)
        return;

    // 获取角色配置中的血量数据
    const UCharacterConfig* CharacterConfig = PlayerCharacter->ShareCharacterConfig();
    if (!CharacterConfig)
        return;

    // 计算血量百分比
    float HealthPercent = static_cast<float>(CharacterConfig->_hp) / CharacterConfig->GetMaxHp();
    HealthPercent = FMath::Clamp(HealthPercent, 0.0f, 1.0f); // 确保在0-1范围内

    // 更新进度条
    HealthBar->SetPercent(HealthPercent);
}

void UCharacterMenu::UpdateManaDisplay()
{
    if (!ManaBar || !PlayerCharacter)
        return;

    const UCharacterConfig* CharacterConfig = PlayerCharacter->ShareCharacterConfig();
    if (!CharacterConfig)
        return;

    // 计算法力值百分比（当前MP / 最大MP）
    float ManaPercent = static_cast<float>(CharacterConfig->_mp) / CharacterConfig->GetMaxMp();
    ManaPercent = FMath::Clamp(ManaPercent, 0.0f, 1.0f);

    ManaBar->SetPercent(ManaPercent);
}

void UCharacterMenu::UpdateCooldownDisplay()
{
    if (!EarthQuakeCooldownText || !PlayerCharacter)
        return;

    // 获取当前剩余冷却时间
    float RemainingCooldown = PlayerCharacter->GetEarthQuakeCooldownRemaining();

    // 格式化文本：冷却中显示"X.XXs"，冷却结束显示空或"就绪"
    FString CooldownText;
    if (RemainingCooldown > 0.0f)
    {
        // 保留两位小数显示
        CooldownText = FString::Printf(TEXT("%.1f"), RemainingCooldown);
    }

    // 设置文本内容
    EarthQuakeCooldownText->SetText(FText::FromString(CooldownText));
}

// 直接从 EventCenter 获取全局 HP 道具数量
void UCharacterMenu::UpdateHpPotionCountDisplay()
{
    if (!HpPotionCountText)
        return;

    // 获取 GameInstance
    Ublack_moneyGameInstance* GameInstance = Cast<Ublack_moneyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!GameInstance) return;

    // 获取 EventCenter
    UEventCenter* EventCenter = GameInstance->GetEventCenter();
    if (!EventCenter) return;

    // 调用我们在 EventCenter 新写的接口 GetToolsNumber()
    // 假设您在 EventCenter.h 中定义的结构体成员名为 hpTools
    int32 TotalHpCount = EventCenter->GetToolsNumber().hpTools;

    // 显示总数
    HpPotionCountText->SetText(FText::FromString(FString::FromInt(TotalHpCount)));
}

// 直接从 EventCenter 获取全局 MP 道具数量
void UCharacterMenu::UpdateMpPotionCountDisplay()
{
    if (!MpPotionCountText)
        return;

    // 获取 GameInstance (同样的过程)
    Ublack_moneyGameInstance* GameInstance = Cast<Ublack_moneyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!GameInstance) return;

    // 获取 EventCenter
    UEventCenter* EventCenter = GameInstance->GetEventCenter();
    if (!EventCenter) return;

    // 获取 mpTools
    int32 TotalMpCount = EventCenter->GetToolsNumber().mpTools;

    // 显示总数
    MpPotionCountText->SetText(FText::FromString(FString::FromInt(TotalMpCount)));
}