using System.Collections.Generic;
using UnityEngine;

namespace GodlyGambit
{
    public class SpawnSoldiersAbility : Ability
    {
        [Header("Soldier Settings")]
        [SerializeField, Range(0, 50)] private int _maxSoldiers = 10;

        [SerializeField] private GameObject _soldierPrefab = null;
        [SerializeField] private GameObject _soldierGhostPrefab = null;

        [Header("Spawn Settings")]
        [SerializeField] private float _spawnRadius = 5f;

        [Header("Visual Settings")]
        [SerializeField] private Material _validMaterial = null;

        [SerializeField] private Material _invalidMaterial = null;

        private readonly List<GameObject> _ghosts = new List<GameObject>();

        private int _lastMaxSoldiers = 0;
        private float _lastSpawnRadius = 0f;

        protected override void Awake()
        {
            base.Awake();

            if (!_soldierPrefab)
            {
                Debug.LogError("No soldier prefab assigned to SpawnSoldiersAbility");
            }

            if (!_soldierGhostPrefab)
            {
                Debug.LogError("No soldier ghost prefab assigned to SpawnSoldiersAbility");
            }

            CreateSoldiers();
            _lastMaxSoldiers = _maxSoldiers;
            _lastSpawnRadius = _spawnRadius;
        }

        private void Update()
        {
            if (IsSettingChanged()) UpdateSettings();
        }

        public override void DeactivateGhosts()
        {
            IsActive = false;

            _ghosts.ForEach(ghost => ghost.SetActive(false));
        }

        public override void Place()
        {
            IsActive = false;

            CreateSoldiers();
            DeactivateGhosts();

            for (int i = 0; i < _maxSoldiers; ++i)
            {
                Instantiate(_soldierPrefab, _ghosts[i].transform.position, GetParent().rotation, DynamicParent);
            }
        }

        public override void Toggle()
        {
            IsActive = !IsActive;
            Debug.Log("Deactivating");

            if (IsActive)
            {
                PlaceInCircle();
            }
            else
            {
                DeactivateGhosts();
            }
        }

        private void CreateSoldiers()
        {
            int ghostAmount = _maxSoldiers - _ghosts.Count; 

            if (ghostAmount <= 0)
            {
                DeactivateGhosts();
                return;
            }

            for (int i = 0; i < ghostAmount; ++i)
            {
                _ghosts.Add(Instantiate(_soldierGhostPrefab, GetParent().position, GetParent().rotation, GetParent()));
                _ghosts[i].SetActive(false);
            }
        }

        private void PlaceInCircle()
        {
            CreateSoldiers();

            float angle = 360f / _maxSoldiers;
            for (int i = 0; i < _maxSoldiers; ++i)
            {
                // Calculate the position of the enemy
                Vector3 pos = GetParent().position + Quaternion.Euler(0f, angle * i, 0f) * Vector3.forward * _spawnRadius;

                // Set the position of the enemy
                _ghosts[i].transform.position = pos;
            }
        }

        private bool IsSettingChanged()
        {
            return _lastMaxSoldiers != _maxSoldiers || _lastSpawnRadius != _spawnRadius;
        }

        private void UpdateSettings()
        {
            PlaceInCircle();

            _lastMaxSoldiers = _maxSoldiers;
            _lastSpawnRadius = _spawnRadius;
        }
    }
}
