# Silkroad UE5 — Ana Yol Haritası

> **Proje:** Silkroad Online'ın UE5 ile yeniden yapımı
> **Hedef:** 2012 seviyesi PC'de (GTX 550 Ti, 4GB RAM) çalışan modern MMO
> **Felsefe:** Görsel değil, oynanış odaklı. Silkroad ruhu + WoW mekanikleri.
> **Son Güncelleme:** 10 Ekim 2026
> **Mevcut Aşama:** Faz 1 (Test + Derleme Düzeltme)

---

## 🎯 PROJE VİZYONU

### Ne Yapıyoruz?
Silkroad Online'ı Unreal Engine 5 ile **sıfırdan** yeniden yapıyoruz. Sadece remake değil; **modern mekaniklerle harmanlanmış** bir deneyim.

### Neden Yapıyoruz?
- Silkroad'ın orijinal sunucuları kapandı, özel sunucular kalitesiz
- Modern MMO'lar (WoW, FFXIV) güzel ama Silkroad'ın **iş sistemi (Tüccar/Hırsız/Avcı)** eşsiz
- 2005'teki "herkesin bilgisayarında çalışan oyun" felsefesi kayboldu

### Kime Yapıyoruz?
- Eski Silkroad oyuncuları (nostalji)
- Modern MMO oyuncuları (oynanış derinliği)
- Düşük sistemli PC sahipleri (erişilebilirlik)
- Mobil oyuncular (cross-platform)

---

## 🎨 TASARIM FELSEFESİ

### Hibrit Model: İki Oyunun En İyisi

| Özellik | Kaynak | Neden? |
|---------|--------|--------|
| **Savaş Akışı** | WoW | GCD, cooldown, cast time, rotation |
| **Defans Sistemi** | Silkroad | PDEF/MDEF, dodge, parry, block |
| **İş Sistemi** | Silkroad | Tüccar, Hırsız, Avcı |
| **Dungeon** | WoW | Instance, heroic, mythic |
| **Job Dungeon** | Silkroad | Hırsız Mağarası vs. |
| **Harita** | Hibrit | Kıta içi seamless, arası loading |
| **Hedefleme** | WoW | Tab-targeting |
| **PvP** | Silkroad | Job PvP (kervan soygunu) |

### Görsel Kalite Hedefi

| Öğe | Hedef |
|-----|-------|
| **Karakter Modeli** | 2010-2012 seviyesi (Skyrim düşük ayar) |
| **Çevre** | 2008-2010 seviyesi (WoW: WotLK) |
| **Işıklandırma** | Statik + basit dinamik |
| **Su** | Basit shader, yansıma yok |
| **Gölge** | Düşük çözünürlük, kısa mesafe |
| **Partikül** | Az, basit |
| **Gökyüzü** | Statik skybox + gradient |

**UE5'i "güzel grafik" için değil, "modern araçlar" için kullanıyoruz:**
- GAS (Gameplay Ability System) → WoW savaş sistemi
- Multiplayer replikasyon → MMO altyapısı
- Blueprint + C++ hibrit → Hızlı geliştirme
- Level Streaming → Seamless dünya

---

## 🏗️ TEKNİK KARARLAR

### Motor ve Araçlar
| Karar | Seçim | Sebep |
|-------|-------|-------|
| Motor | UE 5.4.0 | Düşük sistem desteği, stabil |
| Derleyici | MSVC v14.38 | UE 5.4 uyumlu |
| IDE | VS Code (günlük) + VS 2022 (derleme) | Hafif + Güçlü |
| Versiyon Kontrol | Git + GitHub | Yedek + Paylaşım |

### Render Ayarları (Low-Spec İçin)
| Ayar | Değer |
|------|-------|
| Render Path | Forward Rendering |
| Nanite | Kapalı |
| Lumen | Kapalı |
| Virtual Shadow Maps | Kapalı |
| Anti-Aliasing | FXAA |
| Texture Max | 1024x1024 |
| Draw Call Hedefi | < 1500 |
| Aynı Anda Karakter | Max 20 |

### Ağ Mimarisi
| Konu | Karar |
|------|-------|
| Model | Server Authoritative |
| Tick Rate | 20 tick/saniye |
| Relevancy | 50 metre |
| Bandwidth | 20-50 Kbps/oyuncu |
| Sunucu | Dedicated Server (Linux) |
| İstemci | Windows, Linux, Android, iOS |

---

## 📊 MEVCUT DURUM (Ekim 2026)

> **Kod taraması notu (10 Ekim 2026):** Aşağıdaki listeler gerçek dosya/kod durumuna göre doğrulanmıştır. Önceki "Tamamlanan" listesinde bazı sistemler işaretliydi ama diskte karşılığı yoktu; dürüstlük adına düzeltildi.

### ✅ Gerçekten Tamamlanan (Kodda Doğrulandı)
- [x] UE5 + VS 2022 kurulumu
- [x] İlk C++ projesi (`SilkroadUE`)
- [x] Karakter sınıfı (`SROCharacter`) — istatistikler + savaş formülü dahil
- [x] Oyun modu (`SROGameMode`) — DefaultPawn bağlı
- [x] Kamera (SpringArm + Follow)
- [x] Hareket (WASD + Mouse)
- [x] Zıplama (Space)
- [x] Savaş çekirdeği — dodge/block/parry/crit + PDEF/MDEF defans formülü (`CalculateDamage`)

### ⚠️ Yol Haritasında "Tamamlanan" Yazmış Ama Kodda HENÜZ YOK
- [ ] Düşman sistemi (`SROEnemy`) — dosya yok
- [ ] HUD (`SROHUD`) — dosya yok
- [ ] Ganimet (`SROPickup`) — dosya yok
- [ ] 3 skill (1/2/3 tuşları) — sadece test amaçlı Damage/Heal var
- [ ] XP/Level/Gold sistemi — yok
- [ ] Envanter + kuşanma — yok
- [ ] Basit quest sistemi — yok

### 🚨 KRİTİK: Derleme Durumu
`SROCharacter.cpp` (satır 68-69) şu input'lara bağlanıyor:
```cpp
BindAction("Damage", ...) -> &ASROCharacter::OnDamagePressed
BindAction("Heal",   ...) -> &ASROCharacter::OnHealPressed
```
Ancak **`OnDamagePressed` ve `OnHealPressed` fonksiyonları tanımlı değil** (ne header'da bildirim, ne .cpp'de gövde). Kod bu haliyle **derlenmeyebilir** → Faz 1'in ilk işi bunu düzeltmek.

### ⏸️ Test Edilmedi
- [ ] Hareket çalışıyor mu?
- [ ] Savaş formülü doğru çalışıyor mu?
- [ ] Zıplama çalışıyor mu?
- [ ] Kamera dönüyor mu?

### ❌ Eksikler
- [ ] Multiplayer (replikasyon yok)
- [ ] Database (kalıcı veri yok)
- [ ] GAS (Gameplay Ability System) yok
- [ ] Gerçek karakter modelleri (küp var — `PlaceholderMesh`)
- [ ] Animasyonlar (yürüme, saldırı, cast)
- [ ] Tab-targeting
- [ ] GCD (Global Cooldown) — alanlar var ama aktif kullanım yok
- [ ] Buff/Debuff sistemi
- [ ] Chat sistemi
- [ ] Party/Guild
- [ ] Trade
- [ ] Job sistemi (Tüccar/Hırsız/Avcı)
- [ ] Dungeon sistemi
- [ ] Seamless harita
- [ ] Cross-platform derleme

---

## 🚀 YOL HARİTASI

### 📌 FAZ 1: Test + Stabilize (1 Hafta)
**Amaç:** Mevcut kodun çalıştığından emin olmak

- [ ] 🔴 **Derlemeyi düzelt** — eksik `OnDamagePressed` / `OnHealPressed` fonksiyonlarını yaz (veya input binding'lerini kaldır)
- [ ] Live Coding ile derle → hata var mı?
- [ ] Play → hareket testi (WASD + kamera + zıplama)
- [ ] Savaş testi (Damage/Heal tuşları + formül)
- [ ] **Bug'ları not al**
- [ ] **Bug'ları düzelt**
- [ ] **Git commit yap**

**Çıkış kriteri:** Oyun **hatasız derlenip** çalışıyor.

---

### 📌 FAZ 2: Refactor (1-2 Hafta)
**Amaç:** Kodu MMO'ya hazır hale getirmek

- [ ] `PerformAttack` string → enum
- [ ] EnhancedInput kaldır veya kullan
- [ ] Skill verilerini `DataTable`'a taşı
- [ ] Item verilerini `DataTable`'a taşı
- [ ] Enemy verilerini `DataTable`'a taşı
- [ ] `IDamageable` interface oluştur
- [ ] Server vs Client mantığını ayır
- [ ] Kod yorumlarını düzenle
- [ ] **Git commit (her adımda)**

**Çıkış kriteri:** Kod temiz, data-driven, MMO'ya hazır.

---

### 📌 FAZ 3: Görsel İyileştirme (1 Hafta)
**Amaç:** Küpler yerine gerçek modeller

- [ ] UE5 Mannequin'i projeye ekle
- [ ] `PlaceholderMesh` → `SkeletalMeshComponent`
- [ ] Animation Blueprint oluştur (idle/walk/run/jump)
- [ ] Mannequin animasyonlarını bağla
- [ ] Enemy'ye de aynısını yap
- [ ] Basit silah modeli (elinde kılıç)
- [ ] **Test: Karakter insan gibi görünüyor mu?**

**Çıkış kriteri:** Karakter animasyonlu, insansı görünüm.

---

### 📌 FAZ 4: GAS Entegrasyonu (2-4 Hafta)
**Amaç:** Skill sistemini profesyonelleştirmek

- [ ] `UAttributeSet` oluştur (Health, Mana, AttackPower...)
- [ ] `UGameplayAbility` sınıfları
- [ ] `UGameplayEffect` (hasar, iyileştirme)
- [ ] Mevcut skill'leri GAS'a taşı
- [ ] **GCD (Global Cooldown)** ekle
- [ ] Cooldown UI göstergesi
- [ ] Cast time sistemi
- [ ] **Test: WoW gibi hissettiriyor mu?**

**Çıkış kriteri:** GAS çalışıyor, skill'ler WoW tarzı.


### 📌 FAZ 5: MULTIPLAYER — BÜYÜK ADIM (1-3 Ay)
**Amaç:** Tek kişilikten gerçek MMO'ya geçiş

**5A: Temel Ağ (2-3 Hafta)**
- [ ] Dedicated Server kurulumu
- [ ] Replikasyon temelleri
- [ ] 2 bilgisayarda test
- [ ] Basit "merhaba" replikasyonu

**5B: Karakter Replikasyonu (3-4 Hafta)**
- [ ] Karakter konumu (replike)
- [ ] Can/Mana (replike)
- [ ] Animasyonlar (replike)
- [ ] Movement Component (server-authoritative)
- [ ] Client Prediction (hareket)

**5C: Savaş Replikasyonu (3-4 Hafta)**
- [ ] Hasar server'da hesaplanıyor
- [ ] Skill kullanımı RPC
- [ ] Kritik/dodge/parry sonuçları replike
- [ ] Hile koruması

**5D: Test + Optimizasyon (2-3 Hafta)**
- [ ] 4-8 oyuncu testi
- [ ] Lag simülasyonu
- [ ] Performans ölçümü
- [ ] Bandwidth optimizasyonu

**Çıkış kriteri:** 4 oyuncu aynı sunucuda, sorunsuz oynayabiliyor.

---

### 📌 FAZ 6: MMO Özellikleri (3-6 Ay)
**Amaç:** Gerçek bir MMO olmak

**6A: Kalıcı Veri**
- [ ] MySQL/PostgreSQL veritabanı
- [ ] Karakter kayıt/yükleme
- [ ] Envanter kalıcı
- [ ] Login/Register sistemi

**6B: Sosyal Sistemler**
- [ ] Chat (yerel, parti, lonca, ticaret)
- [ ] Party (grup) sistemi
- [ ] Guild (lonca) sistemi
- [ ] Arkadaş listesi

**6C: Ekonomi**
- [ ] Trade (oyuncular arası)
- [ ] Auction House (pazar)
- [ ] NPC dükkanları

**6D: İçerik**
- [ ] Tab-targeting (hedefleme)
- [ ] Buff/Debuff sistemi
- [ ] Mob çeşitliliği (10-20 tip)
- [ ] Bölgeler (3-5 harita)
- [ ] Boss'lar (5-10)
- [ ] Dungeon (2-3 instance)

**Çıkış kriteri:** 20-30 oyuncu birlikte oynayabiliyor.

---

### 📌 FAZ 7: Silkroad Ruhu (3-6 Ay)
**Amaç:** Silkroad'ı Silkroad yapan şeyler

- [ ] **Job Sistemi:** Tüccar, Hırsız, Avcı
- [ ] **Kervan Sistemi:** NPC + oyuncu escort
- [ ] **Kaçakçılık:** PvP flag sistemi
- [ ] **Job Dungeon:** Hırsız Mağarası vs.
- [ ] **Fortress War:** Loncalar arası savaş
- [ ] **Holy Water Temple:** Klasik Silkroad dungeon
- [ ] **Unique Boss:** Dünya boss'ları
- [ ] **Alchemy:** Item üretme

**Çıkış kriteri:** Silkroad hissiyatı tam.

---

### 📌 FAZ 8: Cross-Platform (3-6 Ay)
**Amaç:** Tüm platformlarda oynanabilir

- [ ] Mobil arayüz (Common UI)
- [ ] Linux derleme
- [ ] Android derleme
- [ ] iOS derleme
- [ ] Cross-platform sunucu

**Çıkış kriteri:** PC + mobil + Linux aynı sunucuda.

---

### 📌 FAZ 9: Cila + Lansman (3-6 Ay)
**Amaç:** Yayına hazır hale getirmek

- [ ] Ses efektleri
- [ ] Müzikler
- [ ] Loading ekranları
- [ ] Ana menü
- [ ] Karakter seçimi
- [ ] Ayarlar (grafik, ses, kontroller)
- [ ] Tutorial
- [ ] Anti-cheat
- [ ] Website + forum
- [ ] Discord sunucusu
- [ ] **Closed Beta**
- [ ] **Open Beta**
- [ ] **Lansman**


---

## 📅 TOPLAM SÜRE TAHMİNİ

| Faz | Süre | Bitiş |
|-----|------|-------|
| Faz 1: Test | 1 hafta | Ekim 2026 |
| Faz 2: Refactor | 2 hafta | Ekim 2026 |
| Faz 3: Görsel | 1 hafta | Kasım 2026 |
| Faz 4: GAS | 1 ay | Aralık 2026 |
| Faz 5: Multiplayer | 3 ay | Mart 2027 |
| Faz 6: MMO Özellikler | 6 ay | Eylül 2027 |
| Faz 7: Silkroad Ruhu | 6 ay | Mart 2028 |
| Faz 8: Cross-Platform | 6 ay | Eylül 2028 |
| Faz 9: Cila + Lansman | 6 ay | Mart 2029 |

**Toplam:** ~2.5 yıl (part-time, ciddi çalışma)

**Not:** Bu süre **tek kişilik** çalışma için geçerlidir. Ekip olursa **1-1.5 yıla** düşer.

---

## 🎯 ŞU ANKİ ÖNCELİKLER

### Bu Hafta (Ekim 2026)
1. 🔴 **Derlemeyi düzelt** (eksik OnDamagePressed/OnHealPressed)
2. **Faz 1:** Mevcut kodu test et (hareket, savaş, zıplama)
3. Bug'ları not al ve düzelt
4. Git commit yap
5. **Faz 2'ye geçiş kararı**

### Bu Ay
1. Refactor (Faz 2)
2. Mannequin modeli ekle (Faz 3)
3. GAS'a başla (Faz 4)

### 3 Ay İçinde
1. GAS tamamlanmış
2. Multiplayer'a başlanmış
3. 2 oyuncu aynı sunucuda test ediliyor

---

## 📋 KARAR GÜNLÜĞÜ

| Tarih | Karar | Sebep |
|-------|-------|-------|
| Eki 2026 | UE 5.4.0 kullan | Düşük sistem desteği |
| Eki 2026 | MSVC v14.38 | UE 5.4 uyumlu |
| Eki 2026 | Forward Rendering | 2012 GPU |
| Eki 2026 | Nanite/Lumen kapalı | Eski GPU desteklemez |
| Eki 2026 | Silkroad + WoW hibrit | İki oyunun en iyisi |
| Eki 2026 | Server Authoritative | Hile koruması |
| Eki 2026 | Test → Refactor → GAS → Multiplayer | Doğru sıra |
| Eki 2026 | Git + GitHub, UE `.gitignore` | Yedek + temiz repo |

---

## 🚨 RİSK LİSTESİ

| Risk | Olasılık | Etki | Çözüm |
|------|----------|------|-------|
| **Motivasyon kaybı** | Yüksek | Yüksek | Küçük hedefler, haftalık ilerleme |
| **Teknik engel** | Orta | Yüksek | Sabır, topluluk, AI desteği |
| **Donanım yetmezliği** | Düşük | Orta | 64GB RAM yükseltmesi |
| **Zaman kısıtı** | Yüksek | Orta | Haftada 20 saat disiplin |
| **Yeniden yazma** | Orta | Yüksek | Doğru sıra, sabır |
| **Cross-platform zorluk** | Yüksek | Orta | Sonraki fazlara bırak |

---

## 💡 PRENSİPLER

1. **Test etmeden ilerleme.** Her yeni özellikten sonra test.
2. **Her gün commit yap.** Kod kaybı en kötü senaryo.
3. **Basit tut.** Karmaşık kod = karmaşık hata.
4. **Dokümantasyon yaz.** Yarın unutacaksın.
5. **Yedek al.** Haftada en az 1 kez fiziksel yedek.
6. **Acele etme.** Yavaş ama doğru > hızlı ama yanlış.
7. **Topluluğa sor.** Takıldığında utanma.
8. **Oyunu oyna.** Kendi oyununu oynamayan geliştirici kötü oyun yapar.

---

## 🎓 ÖĞRENME KAYNAKLARI

| Konu | Kaynak |
|------|--------|
| UE5 Temel | Unreal Engine Learning Portal |
| C++ | learncpp.com |
| GAS | "Gameplay Ability System" - Epic Docs |
| Multiplayer | "Multiplayer in UE5" - Epic Dev Community |
| MMO Mimarisi | "MMO Architecture" - GDC Talks |
| Silkroad Referans | Silkroad Online Wiki |

---

## 📞 NOTLAR

- **Bu dosya canlıdır.** Her önemli karardan sonra güncellenir.
- **Yeni fikirler** buraya eklenir.
- **Tamamlanan işler** ✅ ile işaretlenir.
- **İptal edilen işler** ❌ ile işaretlenir + sebep yazılır.

---

**Son Güncelleme:** 10 Ekim 2026
**Sonraki Güncelleme:** Faz 1 tamamlandığında (derleme düzeltildiğinde)

