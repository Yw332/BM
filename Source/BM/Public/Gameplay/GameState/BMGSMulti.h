#pragma once

#include "CoreMinimal.h"
#include "Gameplay/BMGamePreDefine.h"
#include "Net/UnrealNetwork.h"
#include "Gameplay/BMGameState.h"
#include "BMGSMulti.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGSGameStateChangedSignature, EGameState, NewState);

UCLASS(Abstract, Blueprintable)
class BM_API ABMGSMulti : public ABMGameState
{
	GENERATED_BODY()
public:
	ABMGSMulti();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 由服务器调用，修改状态 + 触发复制
	void SetGameState(EGameState NewState);

	// 给 UI / 其它 Actor 绑定（客户端也能绑定）
	UPROPERTY(BlueprintAssignable)
	FOnGSGameStateChangedSignature OnGameStateChanged;

protected:
	virtual void BeginPlay() override;

	// 当前的游戏状态 —— 在服务器上写入，在客户端上通过 OnRep 更新
	UPROPERTY(ReplicatedUsing = OnRep_GameState)
	EGameState CurrentGameState;

	UFUNCTION()
	void OnRep_GameState();

	// 服务器上用，接 GameMode 的委托
	UFUNCTION()
	void HandleGameModeGameStateChanged(EGameState NewState);
};
