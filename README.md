# IRA Recreation

DirectX9 기반 4인 팀 프로젝트로, 스킬 연출과 전투 흐름 제어를 중심으로 구현했습니다.  
포트폴리오 문서 기준 핵심 기여는 **Time Stop Skill System**과 **Player Combat**입니다.

## Project Info
- **Engine**: DirectX9
- **Development Period**: 30 Days
- **Team Size**: 4
- **Role**: Gameplay / Skill System Programming

## My Core Contributions
- Time Stop Skill System
- Player System (Movement, Dodge, Weapon, Skill)
- Combat Feedback (Hit, Damage Font, Effects)

---

## 1. Time Stop Skill System

레이어 단위 시간 제어를 통해 주변 오브젝트의 업데이트를 정지시키는 특수 스킬 시스템입니다.

### Feature
- 주변 적, 일반 오브젝트, 투사체 등의 움직임을 일괄 정지
- 플레이어 및 예외 객체만 선택적으로 계속 동작하도록 구성

### How it Works
- 시간 정지 상태가 되면 레이어가 관리하는 오브젝트에 `0.f`의 TimeDelta를 전달
- 적, 일반 오브젝트, 투사체 등의 Tick은 사실상 정지 상태로 처리
- 실제 TimeDelta는 레이어에 별도로 저장
- 플레이어와 관련된 예외 객체는 저장된 실제 TimeDelta를 참조해 계속 갱신

### Result
- 주변 오브젝트를 한 번에 멈추면서도
  플레이어와 스킬 관련 객체만 선택적으로 동작시키는 전투 연출 구현
- 전투 흐름을 일시적으로 제어하는 특수 능력 완성

### Design Point
- 오브젝트별로 개별 정지 처리를 넣기보다
  **레이어 단위 TimeDelta 제어**로 시스템을 단순화했습니다.
- 일반 객체와 예외 객체를 분리해 스킬 확장에 유리한 구조로 구성했습니다.

---

## Tech Stack
- C++
- DirectX9
- Gameplay Programming
- Skill System
- Combat System

## What I Focused On
이 프로젝트에서는  
**시간 제어 기반 스킬 구현**, **다양한 전투 시스템 구축**에 집중했습니다.


