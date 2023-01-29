﻿using Client.Domain.Common;

namespace Client.Domain.ValueObjects
{
    public class VitalStats : NotifyPropertyChanged
    {
        private uint hp;
        private uint maxHp;
        private uint mp;
        private uint maxMp;
        private uint cp;
        private uint maxCp;

        public uint Hp { get => hp; set { if (value != hp) { hp = value; OnPropertyChanged("Hp"); } } }
        public uint MaxHp { get => maxHp; set { if (value != maxHp) { maxHp = value; OnPropertyChanged("MaxHp"); } } }
        public uint Mp { get => mp; set { if (value != mp) { mp = value; OnPropertyChanged("Mp"); } } }
        public uint MaxMp { get => maxMp; set { if (value != maxMp) { maxMp = value; OnPropertyChanged("MaxMp"); } } }
        public uint Cp { get => cp; set { if (value != cp) { cp = value; OnPropertyChanged("Cp"); } } }
        public uint MaxCp { get => maxCp; set { if (value != maxCp) { maxCp = value; OnPropertyChanged("MaxCp"); } } }

        public VitalStats(uint hp, uint maxHp, uint mp, uint maxMp, uint cp, uint maxCp)
        {
            this.hp = hp;
            this.maxHp = maxHp;
            this.mp = mp;
            this.maxMp = maxMp;
            this.cp = cp;
            this.maxCp = maxCp;
        }
    }
}