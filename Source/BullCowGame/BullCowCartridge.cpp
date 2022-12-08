// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordsList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();

    if (bGameOver)
    {
        SetupGame();
    }
    else 
    {
        ProcessGuess(Input);     
    }
}

void UBullCowCartridge::SetupGame()
{
    // Welcoming the Player
    PrintLine(TEXT("Welcome to Bulls and Cows game!\n"));
    
    // Get random word from the words list (HiddenWordsList.h)
    // and set other game's variables
    HiddenWord = Words[FMath::RandRange(0, Words.Num() - 1)];
    WordReveal = TEXT("");
    GuessedLetters = TEXT("");
    Lives = HiddenWord.Len() * 1.5;
    bGameOver = false;

    // Generate a string of unrevealed letters
    for (int32 Indx = 0; Indx < HiddenWord.Len(); Indx++) 
    {
        WordReveal += TEXT('_');
    };

    // HIDDEN WORD DEBUGGING
    // PrintLine(TEXT("Hidden word is: %s."), *HiddenWord);
    
    // Instruct the Player with rules
    PrintLine(TEXT("Guess a hidden word.\nThe word is %i letters long."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives before the game is over.\n"), Lives);
    PrintLine(TEXT("Type in your guess and press\nEnter to start..."));

    // const TCHAR HW[] = TEXT("plumbs");
    // PrintLine(TEXT("Character 1 of the hidden word is: %c"), HiddenWord[0]);
    // PrintLine(TEXT("Character 4 of the HW is: %c"), HW[3]);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess.Len() > 0) 
    {
        PrintLine(TEXT("Your input: %s"), *Guess);
    }
    else
    {
        PrintLine(TEXT("Guess a hidden word\nYou have %i lives.\n"), Lives);
        return;
    }
    
    
    if (Guess == HiddenWord) 
    {
        PrintLine(TEXT("Correct! You found the hidden word.\n"));
        EndGame();
        return; 
    } 

    // Validate right number of characters
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("A hidden word has %i characters.\nYou still have %i lives.\nTry again..."), HiddenWord.Len(), Lives);
        return;
    }

    // Validate Isogram
    if (!IsIsogram(Guess)) 
    {
        PrintLine(TEXT("The word is not an isogram!"));
        return;
    }

    // Remove life
    --Lives;
    if (Lives <= 0)
    {
        PrintLine(TEXT("Your guess is incorrect.\nYou are out of lives, the game is over.\n"));
        PrintLine(TEXT("A hidden word was %s"), *HiddenWord);
        EndGame();
        return;
    }

    // Check Bulls and Cows (MY CODE)
    // CheckBullsAndCows(Guess);

    // Show Bulls and Cows and reveal hidden word letters (course code)
    FBullCowCount Score = GetBullsAndCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows\n"), Score.Bulls, Score.Cows);

    // Reveal found letters (MY CODE)
    PrintLine(TEXT("Hidden word: %s"), *WordReveal);
    if (GuessedLetters.Len() > 0) 
    {
        PrintLine(TEXT("Other found letters: %s"), *GuessedLetters);
    }
    else
    {
        PrintLine(TEXT("No other letters found"));
    }
    
    // If more then 0 lifes - start over from the guess 
    PrintLine(TEXT("\nYou have %i lives left. Try again...\n"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    // const TCHAR LattersCheck[] = TEXT(Word);
    for (int32 LetterIndex = 0; LetterIndex < Word.Len(); LetterIndex++) 
    {
        for (int32 NextLetter = LetterIndex + 1; NextLetter < Word.Len(); NextLetter++)
        {
            if (Word[LetterIndex] == Word[NextLetter]) {
                PrintLine(TEXT("Letter %c is repeating!"), Word[LetterIndex]);
                return false;
            }
        }
    }
    return true;
} 

/* void UBullCowCartridge::CheckBullsAndCows(const FString& Word)
{
    int32 Bulls = 0, Cows = 0;

    for (int32 LetterIndex = 0; LetterIndex < Word.Len(); LetterIndex++) 
    {
        for (int32 HiddenLetterIndx = 0; HiddenLetterIndx < Word.Len(); HiddenLetterIndx++)
        {
            if (HiddenWord[HiddenLetterIndx] == Word[LetterIndex]) {
                if (LetterIndex == HiddenLetterIndx) 
                {
                    Bulls++;
                }
                else
                {
                    Cows++;
                } 
            }
        }
    }

    if (Bulls > 0 || Cows > 0)
    {
        PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
    }    
    else 
    {
        PrintLine(TEXT("No similar letters found."));
    }
} */

FBullCowCount UBullCowCartridge::GetBullsAndCows(const FString& Guess)
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) {
            Count.Bulls++;
            WordReveal[GuessIndex] = Guess[GuessIndex];
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++) {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex]) {
                Count.Cows++;
                int32 LineEnd = -1;
                if (GuessedLetters.FindChar(Guess[GuessIndex], LineEnd) != true) 
                {
                    if (!GuessedLetters.IsEmpty()) {
                        GuessedLetters += TEXT(", ");
                    }
                    GuessedLetters += Guess[GuessIndex];
                }
                break;
            }
        }
    }

    return Count;
}
