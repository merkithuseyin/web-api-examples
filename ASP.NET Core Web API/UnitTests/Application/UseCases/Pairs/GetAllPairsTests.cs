using Application.UseCases.Pairs.GetAllPairs;
using Domain.Entities;
using Domain.RepositoryInterfaces;

namespace UnitTests.Application.UseCases.Pairs;

[TestFixture]
public class GetAllPairsTests
{
    private GetAllPairsHandler _getAllPairsHandler = null!;
    private readonly Mock<IPairRepository> _fakePairRepository = new();

    [OneTimeSetUp]
    public void OneTimeSetUp()
    {
        _getAllPairsHandler = new GetAllPairsHandler(_fakePairRepository.Object);
    }

    [TestFixture]
    public class HandleMethod: GetAllPairsTests
    {
        [Test]
        public async Task MethodCalled_ReturnsAllPairs()
        {
            // Arrange
            var fakeDbData = new Pair[]
            {
                new() {Id = 1, Name = "Example_Pair_Name_1", Value = "Example_Pair_Value_1"},
                new() {Id = 2, Name = "Example_Pair_Name_2", Value = "Example_Pair_Value_2"}
            };
            
            _fakePairRepository
                .Setup(x => x.GetAllAsync(It.IsAny<CancellationToken>()))
                .ReturnsAsync(fakeDbData);

            var expected = new GetAllPairsResponse(fakeDbData);

            // Act
            var actual = await _getAllPairsHandler.Handle(new GetAllPairsRequest(), default);

            // Assert
            CollectionAssert.IsNotEmpty(actual.Pairs);
            CollectionAssert.AreEqual(expected.Pairs, actual.Pairs);
        }
        
        [Test]
        public async Task MethodCalled_DbHasNoEntity_ReturnsEmptyCollection()
        {
            // Arrange
            _fakePairRepository
                .Setup(x => x.GetAllAsync(It.IsAny<CancellationToken>()))
                .ReturnsAsync(Array.Empty<Pair>());
            
            // Act
            var actual = await _getAllPairsHandler.Handle(new GetAllPairsRequest(), default);

            // Assert
            CollectionAssert.IsEmpty(actual.Pairs);
        }
    }
}