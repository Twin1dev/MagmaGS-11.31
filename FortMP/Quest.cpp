#include "Quest.h"

void Quests::ProgressQuest(AFortPlayerControllerAthena* PC, UFortQuestItemDefinition* QuestDef, FName BackendName)
{
	PC->GetQuestManager(ESubGame::Athena)->SelfCompletedUpdatedQuest(PC, QuestDef, BackendName, 1, 1, nullptr, true, false);
	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
	for (size_t i = 0; i < PlayerState->PlayerTeam->TeamMembers.Num(); i++)
	{
		auto pc = (AFortPlayerControllerAthena*)PlayerState->PlayerTeam->TeamMembers[i];
		if (pc && pc != PC)
		{
			pc->GetQuestManager(ESubGame::Athena)->SelfCompletedUpdatedQuest(PC, QuestDef, BackendName, 1, 1, PlayerState, true, false);
		}
	}
	auto QuestItem = PC->GetQuestManager(ESubGame::Athena)->GetQuestWithDefinition(QuestDef);
	
	FXPEventEntry test{};
	test.EventXpValue = 5000;//still skunked i will make it so it gets real xp value from datatable
	test.QuestDef = QuestDef;
	test.Time = UGameplayStatics::GetDefaultObj()->GetTimeSeconds(UWorld::GetWorld());
	PC->XPComponent->ChallengeXp += test.EventXpValue;
	PC->XPComponent->TotalXpEarned += test.EventXpValue;
	test.TotalXpEarnedInMatch = PC->XPComponent->TotalXpEarned;
	test.SimulatedXpEvent = QuestDef->GetSingleLineDescription();
	PC->XPComponent->RestXP += test.EventXpValue;
	PC->XPComponent->InMatchProfileVer++;
	PC->XPComponent->OnInMatchProfileUpdate(PC->XPComponent->InMatchProfileVer);
	PC->XPComponent->OnProfileUpdated();
	PC->XPComponent->OnXpUpdated(PC->XPComponent->CombatXp, PC->XPComponent->SurvivalXp, PC->XPComponent->MedalBonusXP, PC->XPComponent->ChallengeXp, PC->XPComponent->MatchXp, PC->XPComponent->TotalXpEarned);
	PC->XPComponent->WaitingQuestXp.Add(test);

	cout << PC->XPComponent->WaitingQuestXp.Num() << endl;
	PC->XPComponent->HighPrioXPEvent(test);	

	auto bruh = QuestDef->GetRewardDescription();
	auto bruh2 = QuestDef->GetDescription();
	auto bruh3 = QuestDef->GetSingleLineDescription();
	auto bruh4 = QuestDef->GetShortDescription();
	Utils::Log(UKismetTextLibrary::GetDefaultObj()->Conv_TextToString(bruh).ToString());
	for (size_t i = 0; i < bruh2.Num(); i++)
	{
		Utils::Log(UKismetTextLibrary::GetDefaultObj()->Conv_TextToString(bruh2[i]).ToString());
	}
	Utils::Log(UKismetTextLibrary::GetDefaultObj()->Conv_TextToString(bruh3).ToString());
	Utils::Log(UKismetTextLibrary::GetDefaultObj()->Conv_TextToString(bruh4).ToString());
}
